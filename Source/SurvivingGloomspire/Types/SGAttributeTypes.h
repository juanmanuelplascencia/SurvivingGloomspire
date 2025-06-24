// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/OutputDevice.h"

#include "CoreMinimal.h"
#include "SGAttributeTypes.generated.h"

/**
 * Enumerates the core attributes used in the game.
 * These map to the six primary ability scores in the Pathfinder/D&D system.
 */
UENUM(BlueprintType)
enum class ESGAttributeType : uint8
{
    STR  UMETA(DisplayName = "Strength"),
    DEX  UMETA(DisplayName = "Dexterity"),
    CON  UMETA(DisplayName = "Constitution"),
    INT  UMETA(DisplayName = "Intelligence"),
    WIS  UMETA(DisplayName = "Wisdom"),
    CHA  UMETA(DisplayName = "Charisma")
};

// Helper function to convert enum to string
static FString GetAttributeTypeAsString(ESGAttributeType AttributeType)
{
    const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/SurvivingGloomspire.ESGAttributeType"), true);
    if (!EnumPtr) return FString("Unknown");
    return EnumPtr->GetNameStringByValue(static_cast<int64>(AttributeType));
}

/**
 * Structure containing data for a character's attribute.
 * Handles base value and calculates the appropriate modifier based on Pathfinder rules.
 */
USTRUCT(BlueprintType)
struct FSGAttributeData
{
    GENERATED_BODY()

    /** The base value of the attribute */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (ClampMin = "1", ClampMax = "30"))
    int32 BaseValue;

    /** The calculated modifier based on the base value */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    int32 Modifier;

    /** Default constructor */
    FSGAttributeData() : BaseValue(10), Modifier(0) 
    {
        UE_LOG(LogTemp, Verbose, TEXT("FSGAttributeData: Created with default values (Base: %d, Modifier: %d)"), BaseValue, Modifier);
    }

    /**
     * Calculates the modifier based on the current base value.
     * Follows Pathfinder rules: (value - 10) / 2 (rounded down)
     */
    void CalculateModifier()
    {
        const int32 OldModifier = Modifier;
        Modifier = FMath::FloorToInt((BaseValue - 10) / 2.0f);
        
        UE_LOG(LogTemp, Verbose, TEXT("FSGAttributeData: Modifier recalculated. Base: %d, Old Mod: %d, New Mod: %d"), 
            BaseValue, OldModifier, Modifier);
    }
};
