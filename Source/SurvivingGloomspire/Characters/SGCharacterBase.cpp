// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGCharacterBase.h"
#include "SGAttributeTypes.h"

// Log category for this class
DEFINE_LOG_CATEGORY_STATIC(LogSGCharacter, Log, All);

// Helper macro for debug logging
#define SG_LOG(Verbosity, Format, ...) \
    if (bEnableDebugLogging) { \
        UE_LOG(LogSGCharacter, Verbosity, TEXT("%s: " Format), *GetName(), ##__VA_ARGS__); \
    }

ASGCharacterBase::ASGCharacterBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    
    // Initialize all attributes with default values
    TArray<FSGAttributeData*> AllAttributes = {
        &Strength,
        &Dexterity,
        &Constitution,
        &Intelligence,
        &Wisdom,
        &Charisma
    };
    
    for (FSGAttributeData* Attr : AllAttributes)
    {
        if (Attr)
        {
            Attr->BaseValue = 10;
            Attr->CalculateModifier();
        }
    }
    
    SG_LOG(Log, "Character initialized with default attributes");
    ASGCharacterBase::DebugLogState(true);
}

void ASGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
    SG_LOG(Log, "Character spawned in level");
    
    // Verify all attributes are properly initialized
    if (Strength.BaseValue == 0 || Dexterity.BaseValue == 0 || 
        Constitution.BaseValue == 0 || Intelligence.BaseValue == 0 || 
        Wisdom.BaseValue == 0 || Charisma.BaseValue == 0)
    {
        SG_LOG(Warning, "One or more attributes not properly initialized!");
    }
}

void ASGCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Example of debug visualization that could be toggled
    if (bEnableDebugLogging)
    {
        // Add any per-frame debug visualization here
    }
}

// Called to bind functionality to input
void ASGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FSGAttributeData ASGCharacterBase::GetAttribute(ESGAttributeType AttributeType) const
{
    const FSGAttributeData* AttributePtr = nullptr;
    
    switch (AttributeType)
    {
    case ESGAttributeType::STR: AttributePtr = &Strength; break;
    case ESGAttributeType::DEX: AttributePtr = &Dexterity; break;
    case ESGAttributeType::CON: AttributePtr = &Constitution; break;
    case ESGAttributeType::INT: AttributePtr = &Intelligence; break;
    case ESGAttributeType::WIS: AttributePtr = &Wisdom; break;
    case ESGAttributeType::CHA: AttributePtr = &Charisma; break;
    default:
        SG_LOG(Warning, "GetAttribute: Invalid attribute type: %d", static_cast<int32>(AttributeType));
        return FSGAttributeData();
    }
    
    if (!AttributePtr)
    {
        SG_LOG(Error, "GetAttribute: Null attribute pointer for type: %s", *GetAttributeName(AttributeType));
        return FSGAttributeData();
    }
    
    return *AttributePtr;
}

void ASGCharacterBase::SetBaseAttribute(ESGAttributeType AttributeType, int32 NewValue)
{
    FSGAttributeData* AttributeToModify = nullptr;
    const int32 ClampedValue = FMath::Clamp(NewValue, 1, 30); // Standard D&D/Pathfinder range
    
    switch (AttributeType)
    {
    case ESGAttributeType::STR: AttributeToModify = &Strength; break;
    case ESGAttributeType::DEX: AttributeToModify = &Dexterity; break;
    case ESGAttributeType::CON: AttributeToModify = &Constitution; break;
    case ESGAttributeType::INT: AttributeToModify = &Intelligence; break;
    case ESGAttributeType::WIS: AttributeToModify = &Wisdom; break;
    case ESGAttributeType::CHA: AttributeToModify = &Charisma; break;
    default:
        SG_LOG(Warning, "SetBaseAttribute: Invalid attribute type: %d", static_cast<int32>(AttributeType));
        return;
    }
    
    if (AttributeToModify)
    {
        const int32 OldValue = AttributeToModify->BaseValue;
        if (OldValue != ClampedValue)
        {
            AttributeToModify->BaseValue = ClampedValue;
            AttributeToModify->CalculateModifier();
            
            SG_LOG(Log, "Attribute %s changed: %d -> %d (Modifier: %d)", 
                *GetAttributeName(AttributeType), 
                OldValue, 
                ClampedValue, 
                AttributeToModify->Modifier);
                
            // Broadcast any attribute change delegates here if needed
            // OnAttributeChanged.Broadcast(AttributeType, OldValue, ClampedValue);
        }
    }
    else
    {
        SG_LOG(Error, "SetBaseAttribute: Failed to find attribute for type: %s", *GetAttributeName(AttributeType));
    }
}

void ASGCharacterBase::CalculateAllModifiers()
{
    TArray<TPair<FString, FSGAttributeData*>> Attributes = {
        {TEXT("Strength"), &Strength},
        {TEXT("Dexterity"), &Dexterity},
        {TEXT("Constitution"), &Constitution},
        {TEXT("Intelligence"), &Intelligence},
        {TEXT("Wisdom"), &Wisdom},
        {TEXT("Charisma"), &Charisma}
    };
    
    for (auto& Pair : Attributes)
    {
        if (Pair.Value)
        {
            const int32 OldModifier = Pair.Value->Modifier;
            Pair.Value->CalculateModifier();
            
            if (OldModifier != Pair.Value->Modifier)
            {
                SG_LOG(Log, "%s modifier updated: %d -> %d", 
                    *Pair.Key, 
                    OldModifier, 
                    Pair.Value->Modifier);
            }
        }
    }
}

int32 ASGCharacterBase::GetAttributeModifier(ESGAttributeType AttributeType) const
{
    const FSGAttributeData* AttributePtr = nullptr;
    
    switch (AttributeType)
    {
    case ESGAttributeType::STR: AttributePtr = &Strength; break;
    case ESGAttributeType::DEX: AttributePtr = &Dexterity; break;
    case ESGAttributeType::CON: AttributePtr = &Constitution; break;
    case ESGAttributeType::INT: AttributePtr = &Intelligence; break;
    case ESGAttributeType::WIS: AttributePtr = &Wisdom; break;
    case ESGAttributeType::CHA: AttributePtr = &Charisma; break;
    default:
        SG_LOG(Warning, "GetAttributeModifier: Invalid attribute type: %d", static_cast<int32>(AttributeType));
        return 0;
    }
    
    if (!AttributePtr)
    {
        SG_LOG(Error, "GetAttributeModifier: Null attribute pointer for type: %s", *GetAttributeName(AttributeType));
        return 0;
    }
    
    return AttributePtr->Modifier;
}

int32 ASGCharacterBase::GetAttributeValue(ESGAttributeType AttributeType) const
{
    const FSGAttributeData* AttributePtr = nullptr;
    
    switch (AttributeType)
    {
    case ESGAttributeType::STR: AttributePtr = &Strength; break;
    case ESGAttributeType::DEX: AttributePtr = &Dexterity; break;
    case ESGAttributeType::CON: AttributePtr = &Constitution; break;
    case ESGAttributeType::INT: AttributePtr = &Intelligence; break;
    case ESGAttributeType::WIS: AttributePtr = &Wisdom; break;
    case ESGAttributeType::CHA: AttributePtr = &Charisma; break;
    default:
        SG_LOG(Warning, "GetAttributeValue: Invalid attribute type: %d", static_cast<int32>(AttributeType));
        return 0;
    }
    
    if (!AttributePtr)
    {
        SG_LOG(Error, "GetAttributeValue: Null attribute pointer for type: %s", *GetAttributeName(AttributeType));
        return 0;
    }
    
    return AttributePtr->BaseValue;
}

FString ASGCharacterBase::GetAttributeName(ESGAttributeType AttributeType)
{
    return GetAttributeTypeAsString(AttributeType);
}

void ASGCharacterBase::DebugLogState(bool bForce) const
{
    if (!bEnableDebugLogging && !bForce)
    {
        return;
    }
    
    FString DebugString = FString::Printf(TEXT("=== Character State: %s ===\n"), *GetName());
    
    const TArray<TPair<FString, const FSGAttributeData*>> Attributes = {
        {TEXT("STR"), &Strength},
        {TEXT("DEX"), &Dexterity},
        {TEXT("CON"), &Constitution},
        {TEXT("INT"), &Intelligence},
        {TEXT("WIS"), &Wisdom},
        {TEXT("CHA"), &Charisma}
    };
    
    for (const auto& Pair : Attributes)
    {
        if (Pair.Value)
        {
            DebugString += FString::Printf(TEXT("%s: %2d (Mod: %+d)\n"), 
                *Pair.Key, 
                Pair.Value->BaseValue, 
                Pair.Value->Modifier);
        }
    }
    
    UE_LOG(LogSGCharacter, Log, TEXT("\n%s"), *DebugString);
}
