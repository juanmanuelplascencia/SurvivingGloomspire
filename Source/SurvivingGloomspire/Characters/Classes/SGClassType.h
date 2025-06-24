// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGClassType.generated.h"

/**
 * Core Pathfinder 1e base classes
 */
UENUM(BlueprintType)
enum class ESGClassType : uint8
{
    None        UMETA(DisplayName = "None"),
    Barbarian   UMETA(DisplayName = "Barbarian"),
    Bard        UMETA(DisplayName = "Bard"),
    Cleric      UMETA(DisplayName = "Cleric"),
    Druid       UMETA(DisplayName = "Druid"),
    Fighter     UMETA(DisplayName = "Fighter"),
    Monk        UMETA(DisplayName = "Monk"),
    Paladin     UMETA(DisplayName = "Paladin"),
    Ranger      UMETA(DisplayName = "Ranger"),
    Rogue       UMETA(DisplayName = "Rogue"),
    Sorcerer    UMETA(DisplayName = "Sorcerer"),
    Wizard      UMETA(DisplayName = "Wizard")
};

// Helper function to convert enum to string
static FString GetClassTypeAsString(ESGClassType ClassType)
{
    const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/SurvivingGloomspire.ESGClassType"), true);
    if (!EnumPtr) return FString("Unknown");
    return EnumPtr->GetNameStringByValue(static_cast<int64>(ClassType));
}
