// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGCharacterBase.h"
#include "SGAttributeTypes.h"
#include "SGDerivedAttributesTypes.h"

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
    
    // Initialize default attribute values
    InitializeDefaultAttributes();
    
    // Calculate initial modifiers and derived attributes
    CalculateAllModifiers();
}

void ASGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
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
    
    // Initialize derived attributes with default values
    DerivedAttributes = FSGDerivedAttributes();
    
    // Set default derived attribute base values
    DerivedAttributeBaseValues = FSDerivedAttributeBaseValues();
    DerivedAttributeBaseValues.BaseHitPoints = 10; // Base HP for level 1 character
    DerivedAttributeBaseValues.BaseFortitudeSave = 2;
    DerivedAttributeBaseValues.BaseReflexSave = 0;
    DerivedAttributeBaseValues.BaseWillSave = 2;
    
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
    if (!Attributes.Contains(ESGAttributeType::CON) || !Attributes.Contains(ESGAttributeType::DEX))
    {
        SG_LOG(Error, TEXT("Missing required attributes for calculating derived attributes"));
        return;
    }

    const int32 ConMod = GetAttributeModifier(ESGAttributeType::CON);
    const int32 DexMod = GetAttributeModifier(ESGAttributeType::DEX);
    const int32 WisMod = GetAttributeModifier(ESGAttributeType::WIS);

    // Calculate Max HP (Base + CON mod * level, minimum 1 per level)
    const int32 MaxHP = FMath::Max(1, DerivedAttributeBaseValues.BaseHitPoints + ConMod);
    
    // Calculate AC (10 + armor + shield + DEX mod + size + natural + deflection + dodge + misc)
    const int32 AC = 10 + 
                     DerivedAttributeBaseValues.ArmorBonus + 
                     DerivedAttributeBaseValues.ShieldBonus + 
                     DexMod + 
                     DerivedAttributeBaseValues.NaturalArmor + 
                     DerivedAttributeBaseValues.DeflectionBonus + 
                     DerivedAttributeBaseValues.DodgeBonus;

    // Update derived attributes
    DerivedAttributes.MaxHitPoints = FMath::Max(1, MaxHP);
    
    // Only auto-fill HP if it hasn't been set yet or is invalid
    if (DerivedAttributes.HitPoints <= 0 || DerivedAttributes.HitPoints > DerivedAttributes.MaxHitPoints)
    {
        DerivedAttributes.HitPoints = DerivedAttributes.MaxHitPoints;
    }
    
    // Calculate different AC types
    DerivedAttributes.ArmorClass = FMath::Max(10, AC);
    
    // Touch AC is AC without armor, shield, and natural armor bonuses
    DerivedAttributes.TouchArmorClass = FMath::Max(10, 
        10 + DexMod + DerivedAttributeBaseValues.DeflectionBonus + DerivedAttributeBaseValues.DodgeBonus);
        
    // Flat-footed AC is AC without DEX and dodge bonuses
    DerivedAttributes.FlatFootedArmorClass = FMath::Max(10, 
        10 + DerivedAttributeBaseValues.ArmorBonus + DerivedAttributeBaseValues.ShieldBonus + 
        DerivedAttributeBaseValues.NaturalArmor + DerivedAttributeBaseValues.DeflectionBonus);

    // Calculate saving throws
    DerivedAttributes.FortitudeSave = DerivedAttributeBaseValues.BaseFortitudeSave + ConMod;
    DerivedAttributes.ReflexSave = DerivedAttributeBaseValues.BaseReflexSave + DexMod;
    DerivedAttributes.WillSave = DerivedAttributeBaseValues.BaseWillSave + WisMod;
    
    SG_LOG(Verbose, TEXT("Recalculated derived attributes: %s"), *DerivedAttributes.ToString());
}

bool ASGCharacterBase::ApplyDamage(int32 Amount)
{
    if (Amount <= 0)
    {
        SG_LOG(Warning, TEXT("Attempted to apply non-positive damage: %d"), Amount);
        return false;
    }

    const int32 OldHP = DerivedAttributes.HitPoints;
    
    // First reduce temporary hit points if any
    if (DerivedAttributes.TemporaryHitPoints > 0)
    {
        const int32 DamageToTempHP = FMath::Min(DerivedAttributes.TemporaryHitPoints, Amount);
        DerivedAttributes.TemporaryHitPoints -= DamageToTempHP;
        Amount -= DamageToTempHP;
        
        if (Amount <= 0)
        {
            SG_LOG(Log, TEXT("Absorbed %d damage with temp HP, remaining temp HP: %d"), 
                DamageToTempHP, DerivedAttributes.TemporaryHitPoints);
            return false;
        }
    }
    
    // Apply remaining damage to actual HP
    DerivedAttributes.HitPoints = FMath::Max(0, DerivedAttributes.HitPoints - Amount);
    
    SG_LOG(Log, TEXT("Took %d damage (was %d, now %d/%d)"), 
        Amount, OldHP, DerivedAttributes.HitPoints, DerivedAttributes.MaxHitPoints);
    
    // Return true if character is now at or below 0 HP
    return DerivedAttributes.HitPoints <= 0;
}

int32 ASGCharacterBase::ApplyHealing(int32 Amount)
{
    if (Amount <= 0)
    {
        SG_LOG(Warning, TEXT("Attempted to apply non-positive healing: %d"), Amount);
        return 0;
    }
    
    const int32 OldHP = DerivedAttributes.HitPoints;
    const int32 NewHP = FMath::Min(DerivedAttributes.HitPoints + Amount, DerivedAttributes.MaxHitPoints);
    const int32 ActualHealing = NewHP - OldHP;
    
    if (ActualHealing > 0)
    {
        DerivedAttributes.HitPoints = NewHP;
        SG_LOG(Log, TEXT("Healed %d HP (was %d, now %d/%d)"), 
            ActualHealing, OldHP, DerivedAttributes.HitPoints, DerivedAttributes.MaxHitPoints);
    }
    
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
    
    FString LogOutput = FString::Printf(TEXT("%s State:\n\n"), *GetName());
    
    // Log all attributes
    LogOutput += TEXT("Base Attributes:\n");
    for (const auto& Elem : Attributes)
    {
        const FString AttrName = GetAttributeName(Elem.Key);
        LogOutput += FString::Printf(TEXT("  %s: %2d (Mod: %+d)\n"), 
            *AttrName, Elem.Value.BaseValue, Elem.Value.Modifier);
    }
    
    // Log derived attributes
    LogOutput += TEXT("\nDerived Attributes:\n");
    LogOutput += FString::Printf(TEXT("  %s\n"), *DerivedAttributes.ToString());
    
    // Log base values
    LogOutput += TEXT("\nBase Values:\n");
    LogOutput += FString::Printf(TEXT("  Base HP: %d\n"), DerivedAttributeBaseValues.BaseHitPoints);
    LogOutput += FString::Printf(TEXT("  Saves (F/R/W): %d/%d/%d\n"), 
        DerivedAttributeBaseValues.BaseFortitudeSave,
        DerivedAttributeBaseValues.BaseReflexSave,
        DerivedAttributeBaseValues.BaseWillSave);
    LogOutput += FString::Printf(TEXT("  AC Bonuses: Armor=%d, Shield=%d, Natural=%d, Deflection=%d, Dodge=%d\n"),
        DerivedAttributeBaseValues.ArmorBonus,
        DerivedAttributeBaseValues.ShieldBonus,
        DerivedAttributeBaseValues.NaturalArmor,
        DerivedAttributeBaseValues.DeflectionBonus,
        DerivedAttributeBaseValues.DodgeBonus);
    
    UE_LOG(LogSGCharacter, Log, TEXT("\n%s"), *LogOutput);
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
