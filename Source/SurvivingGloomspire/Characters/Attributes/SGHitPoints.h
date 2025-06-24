// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGHitPoints.generated.h"

/**
 * Structure containing hit point related attributes
 */
USTRUCT(BlueprintType)
struct FSGHitPoints
{
    GENERATED_BODY()

    FSGHitPoints()
        : Current(0)
        , Max(0)
        , Temporary(0)
    {}

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 Current;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 Max;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 Temporary;

    /**
     * Gets the total effective hit points (current + temporary)
     */
    int32 GetTotal() const { return FMath::Max(0, Current) + FMath::Max(0, Temporary); }

    /**
     * Applies damage, reducing temporary hit points first, then current hit points
     * @param Damage Amount of damage to apply
     * @return The actual damage taken after accounting for temporary hit points
     */
    int32 ApplyDamage(int32 Damage)
    {
        int32 DamageToTemp = 0;
        if (Temporary > 0)
        {
            DamageToTemp = FMath::Min(Temporary, Damage);
            Temporary -= DamageToTemp;
            Damage -= DamageToTemp;
            
            if (Damage <= 0) return DamageToTemp;
        }
        
        const int32 DamageToCurrent = FMath::Min(Current, Damage);
        Current -= DamageToCurrent;
        
        return DamageToTemp + DamageToCurrent;
    }

    /**
     * Heals the character, restoring current hit points up to the maximum
     * @param Amount Amount to heal
     * @return The actual amount healed
     */
    int32 Heal(int32 Amount)
    {
        const int32 HealAmount = FMath::Min(Amount, FMath::Max(0, Max - Current));
        Current += HealAmount;
        return HealAmount;
    }
};
