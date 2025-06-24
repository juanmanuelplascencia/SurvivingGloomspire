// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGAttributeType.generated.h"

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
