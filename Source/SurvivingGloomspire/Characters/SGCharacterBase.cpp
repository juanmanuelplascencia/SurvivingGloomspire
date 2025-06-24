// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGCharacterBase.h"
#include "SGAttributeType.h"
#include "SGAttributeData.h"
#include "SGHitPoints.h"
#include "SGArmorClass.h"
#include "SGSavingThrows.h"
#include "SGClassType.h"
#include "SGSkillComponent.h"

// Define the log category for this class
DEFINE_LOG_CATEGORY_STATIC(LogSGCharacter, Log, All);

// Debug logging macro for this class
#define SG_LOG(Verbosity, Format, ...) \
    if (bEnableDebugLogging) { \
        UE_LOG(LogSGCharacter, Verbosity, TEXT("%s: ") Format, *GetName(), ##__VA_ARGS__); \
    }

// ======================================================================
// Construction & Initialization
// ======================================================================

ASGCharacterBase::ASGCharacterBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
    // Create and initialize the class component
    ClassComponent = CreateDefaultSubobject<USGClassComponent>(TEXT("ClassComponent"));
    
    // Create and initialize the skill component
    SkillComponent = CreateDefaultSubobject<USGSkillComponent>(TEXT("SkillComponent"));
    
    // Initialize default attribute values
    InitializeDefaultAttributes();
    
    // Calculate initial modifiers and derived attributes
    CalculateAllModifiers();
}

void ASGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize the skill component with this character
    if (SkillComponent)
    {
        SkillComponent->Initialize(this);
    }
    
    // Log initial state
    DebugLogState(true);
}

void ASGCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGCharacterBase::InitializeDefaultAttributes()
{
    // Clear any existing attributes
    Attributes.Reset();
    
    // Set default attribute values (10 is average in Pathfinder)
    Attributes.Add(ESGAttributeType::STR, FSGAttributeData());
    Attributes.Add(ESGAttributeType::DEX, FSGAttributeData());
    Attributes.Add(ESGAttributeType::CON, FSGAttributeData());
    Attributes.Add(ESGAttributeType::INT, FSGAttributeData());
    Attributes.Add(ESGAttributeType::WIS, FSGAttributeData());
    Attributes.Add(ESGAttributeType::CHA, FSGAttributeData());
    
    // Initialize hit points
    HitPoints = FSGHitPoints();
    HitPoints.Max = 10; // Base HP for level 1 character
    HitPoints.Current = 10;
    
    // Initialize armor class
    ArmorClass = FSGArmorClass();
    
    // Initialize saving throws
    SavingThrows = FSGSavingThrows();
    SavingThrows.Fortitude.BaseSave = 2;
    SavingThrows.Reflex.BaseSave = 0;
    SavingThrows.Will.BaseSave = 2;
    
    // Set base hit points
    BaseHitPoints = 10;
    
    SG_LOG(Log, TEXT("Initialized default attributes"));
}

// ======================================================================
// Attribute Management
// ======================================================================

FSGAttributeData ASGCharacterBase::GetAttribute(ESGAttributeType AttributeType) const
{
    const FSGAttributeData* FoundAttr = Attributes.Find(AttributeType);
    if (FoundAttr)
    {
        return *FoundAttr;
    }
    
    SG_LOG(Warning, TEXT("Attribute type %d not found, returning default"), static_cast<int32>(AttributeType));
    return FSGAttributeData();
}

void ASGCharacterBase::SetBaseAttribute(ESGAttributeType AttributeType, int32 NewValue)
{
    // Clamp value between 1 and 30 (Pathfinder rules)
    const int32 ClampedValue = FMath::Clamp(NewValue, 1, 30);
    
    if (FSGAttributeData* Attribute = Attributes.Find(AttributeType))
    {
        if (Attribute->BaseValue != ClampedValue)
        {
            const int32 OldValue = Attribute->BaseValue;
            Attribute->BaseValue = ClampedValue;
            Attribute->CalculateModifier();
            
            SG_LOG(Log, TEXT("Attribute %s changed: %d -> %d (Modifier: %d)"), 
                *GetAttributeName(AttributeType), 
                OldValue, 
                ClampedValue, 
                Attribute->Modifier);
                
            // Recalculate derived attributes that depend on this attribute
            CalculateDerivedAttributes();
            
            // Notify derived classes that an attribute changed
            OnAttributeChanged(AttributeType);
        }
    }
    else
    {
        SG_LOG(Error, TEXT("Failed to find attribute for type: %s"), *GetAttributeName(AttributeType));
    }
}

void ASGCharacterBase::CalculateAllModifiers()
{
    // Recalculate all attribute modifiers
    for (auto& Elem : Attributes)
    {
        Elem.Value.CalculateModifier();
    }
    
    // Recalculate derived attributes after all modifiers are updated
    CalculateDerivedAttributes();
    
    SG_LOG(Log, TEXT("Recalculated all attribute modifiers and derived attributes"));
}

int32 ASGCharacterBase::GetAttributeModifier(ESGAttributeType AttributeType) const
{
    if (const FSGAttributeData* Attribute = Attributes.Find(AttributeType))
    {
        return Attribute->Modifier;
    }
    
    SG_LOG(Warning, TEXT("Attribute type %d not found, returning 0"), static_cast<int32>(AttributeType));
    return 0;
}

int32 ASGCharacterBase::GetAttributeValue(ESGAttributeType AttributeType) const
{
    if (const FSGAttributeData* Attribute = Attributes.Find(AttributeType))
    {
        return Attribute->BaseValue;
    }
    
    SG_LOG(Warning, TEXT("Attribute type %d not found, returning 0"), static_cast<int32>(AttributeType));
    return 0;
}

FString ASGCharacterBase::GetAttributeName(ESGAttributeType AttributeType)
{
    // Use GetStaticEnum with the enum type directly
    const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/SurvivingGloomspire.ESGAttributeType"), true);
    if (!EnumPtr)
    {
        // Fallback to static find if the first approach fails
        EnumPtr = FindFirstObject<UEnum>(TEXT("ESGAttributeType"));
        if (!EnumPtr)
        {
            return FString("Unknown");
        }
    }
    
    FString DisplayName = EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(AttributeType)).ToString();
    return DisplayName.IsEmpty() ? FString("Unknown") : DisplayName;
}

// ======================================================================
// Derived Attributes & Combat
// ======================================================================

void ASGCharacterBase::CalculateDerivedAttributes()
{
    if (!Attributes.Contains(ESGAttributeType::CON) || !Attributes.Contains(ESGAttributeType::DEX) || !Attributes.Contains(ESGAttributeType::WIS))
    {
        SG_LOG(Error, TEXT("Missing required attributes for calculating derived attributes"));
        return;
    }

    const int32 ConMod = GetAttributeModifier(ESGAttributeType::CON);
    // Update hit points
    const int32 NewMaxHP = FMath::Max(1, BaseHitPoints + ConMod);
    HitPoints.Max = NewMaxHP;
    HitPoints.Current = FMath::Min(HitPoints.Current, NewMaxHP);
    
    SG_LOG(Verbose, TEXT("Recalculated derived attributes"));
    DebugLogState();
}

bool ASGCharacterBase::ApplyDamage(int32 Amount)
{
    if (Amount <= 0)
    {
        SG_LOG(Warning, TEXT("Attempted to apply non-positive damage: %d"), Amount);
        return false;
    }

    const int32 OldHP = HitPoints.Current;
    const int32 DamageTaken = HitPoints.ApplyDamage(Amount);
    const bool bIsDefeated = (HitPoints.Current <= 0);
    
    SG_LOG(Log, TEXT("Took %d damage (HP: %d -> %d)"), DamageTaken, OldHP, HitPoints.Current);
    
    // Check if character is dead
    if (bIsDefeated)
    {
        SG_LOG(Log, TEXT("Character has been defeated!"));
    }
    
    return bIsDefeated;
}

int32 ASGCharacterBase::ApplyHealing(int32 Amount)
{
    if (Amount <= 0 || HitPoints.Current >= HitPoints.Max)
    {
        return 0; // No healing to apply or already at max HP
    }
    
    const int32 OldHP = HitPoints.Current;
    const int32 ActualHealing = HitPoints.Heal(Amount);
    
    SG_LOG(Log, TEXT("Healed for %d (HP: %d -> %d)"), ActualHealing, OldHP, HitPoints.Current);
    
    return ActualHealing;
}

// ======================================================================
// Debug & Development
// ======================================================================

void ASGCharacterBase::DebugLogState(bool bForce) const
{
    if (!bEnableDebugLogging && !bForce)
    {
        return;
    }
    
    // Log basic character info
    FString DebugString = FString::Printf(TEXT("=== %s ===\n"), *GetName());
    DebugString += FString::Printf(TEXT("Position: %s\n"), *GetActorLocation().ToString());
    
    // Log class and level information
    if (ClassComponent)
    {
        DebugString += TEXT("\nClass & Level:\n");
        const TArray<FSGCharacterClassLevel>& ClassLevels = ClassComponent->GetClassLevels();
        for (const FSGCharacterClassLevel& ClassLevel : ClassLevels)
        {
            DebugString += FString::Printf(TEXT("  %s %d\n"),
                *GetClassTypeAsString(ClassLevel.ClassType),
                ClassLevel.Level);
        }
        DebugString += FString::Printf(TEXT("Total Level: %d\n"), ClassComponent->GetTotalLevels());
        DebugString += FString::Printf(TEXT("XP: %d/%d\n"), 
            ClassComponent->GetCurrentXP(),
            ClassComponent->GetXPForNextLevel());
    }
    
    // Log attributes
    DebugString += TEXT("\nAttributes:\n");
    for (const auto& Elem : Attributes)
    {
        const ESGAttributeType AttrType = Elem.Key;
        const FSGAttributeData& Attr = Elem.Value;
        DebugString += FString::Printf(TEXT("  %s: %d (Mod: %+d)\n"),
            *GetAttributeName(AttrType),
            Attr.BaseValue,
            GetAttributeModifier(AttrType));
    }
    
    // Log derived attributes
    DebugString += TEXT("\nDerived Attributes:\n");
    DebugString += FString::Printf(TEXT("  HP: %d/%d (Temp: %d)\n"),
        HitPoints.Current,
        HitPoints.Max,
        HitPoints.Temporary);
        
    // Log skills if we have a skill component
    if (SkillComponent)
    {
        DebugString += TEXT("\nSkills:\n");
        const TMap<ESGSkillType, FSGSkillData>& AllSkills = SkillComponent->GetAllSkills();
        
        for (const auto& Elem : AllSkills)
        {
            const ESGSkillType SkillType = Elem.Key;
            const FSGSkillData& SkillData = Elem.Value;
            
            if (SkillData.Ranks > 0 || SkillData.ClassSkill)
            {
                const int32 TotalBonus = SkillComponent->GetSkillBonus(SkillType);
                const FString SkillName = USGSkillComponent::GetSkillDisplayName(SkillType);
                const ESGAttributeType KeyAbility = USGSkillComponent::GetKeyAbilityForSkill(SkillType);
                const FString AbilityName = GetAttributeName(KeyAbility);
                
                DebugString += FString::Printf(TEXT("  %s: %+d (%s %+d + %d ranks%s)\n"),
                    *SkillName,
                    TotalBonus,
                    *AbilityName,
                    GetAttributeModifier(KeyAbility),
                    SkillData.Ranks,
                    SkillData.ClassSkill ? TEXT(" + 3 class") : TEXT(""));
            }
        }
    }
        
    // Calculate AC values for display
    const int32 DexMod = GetAttributeModifier(ESGAttributeType::DEX);
    const int32 TotalAC = ArmorClass.CalculateTotalAC(DexMod);
    const int32 TouchAC = ArmorClass.CalculateTouchAC(DexMod);
    const int32 FlatFootedAC = ArmorClass.CalculateFlatFootedAC();
    
    DebugString += FString::Printf(TEXT("  AC: %d (Touch: %d, FF: %d)\n"), 
        TotalAC,
        TouchAC,
        FlatFootedAC);
        
    // Calculate saving throws for display
    const int32 ConMod = GetAttributeModifier(ESGAttributeType::CON);
    const int32 WisMod = GetAttributeModifier(ESGAttributeType::WIS);
    
    DebugString += FString::Printf(TEXT("  Saves - Fort: %+d, Ref: %+d, Will: %+d\n"),
        SavingThrows.Fortitude.CalculateTotal(ConMod),
        SavingThrows.Reflex.CalculateTotal(DexMod),
        SavingThrows.Will.CalculateTotal(WisMod));
    
    UE_LOG(LogSGCharacter, Log, TEXT("%s"), *DebugString);
}

// ======================================================================
// Protected Helpers
// ======================================================================

void ASGCharacterBase::OnAttributeChanged(ESGAttributeType AttributeType)
{
    // Base implementation does nothing, can be overridden by derived classes
    // This is called after an attribute changes and derived attributes are recalculated
    SG_LOG(Verbose, TEXT("Attribute changed: %s"), *GetAttributeName(AttributeType));
}
