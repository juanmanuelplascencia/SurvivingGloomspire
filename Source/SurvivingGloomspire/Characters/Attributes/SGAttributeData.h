// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGAttributeData.generated.h"

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
