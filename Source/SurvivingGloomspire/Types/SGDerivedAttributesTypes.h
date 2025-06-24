// Copyright 2024 SurvivingGloomspire. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGDerivedAttributesTypes.generated.h"

/**
 * Structure containing all derived character attributes that are calculated from base attributes.
 * Follows Pathfinder 1e rules for attribute calculations.
 */
USTRUCT(BlueprintType)
struct FSGDerivedAttributes
{
    GENERATED_BODY()

    FSGDerivedAttributes()
        : HitPoints(0)
        , MaxHitPoints(0)
        , TemporaryHitPoints(0)
        , ArmorClass(10) // Base AC is 10 in Pathfinder
        , TouchArmorClass(10)
        , FlatFootedArmorClass(10)
        , FortitudeSave(0)
        , ReflexSave(0)
        , WillSave(0)
    {}

    // Hit Points
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 HitPoints;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 MaxHitPoints;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|HP")
    int32 TemporaryHitPoints;

    // Armor Class
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Defense")
    int32 ArmorClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Defense")
    int32 TouchArmorClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Defense")
    int32 FlatFootedArmorClass;

    // Saving Throws
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Saves")
    int32 FortitudeSave;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Saves")
    int32 ReflexSave;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Saves")
    int32 WillSave;

    /**
     * Returns a string representation of all derived attributes for debugging purposes.
     * @return Formatted string with all derived attributes.
     */
    FString ToString() const
    {
        return FString::Printf(
            TEXT("HP: %d/%d (Temp: %d), AC: %d (Touch: %d, FF: %d), Saves (F/R/W): %d/%d/%d"),
            HitPoints, MaxHitPoints, TemporaryHitPoints,
            ArmorClass, TouchArmorClass, FlatFootedArmorClass,
            FortitudeSave, ReflexSave, WillSave
        );
    }
};

/**
 * Structure to hold base values used for derived attribute calculations.
 * These values come from character level, class, and other sources.
 */
USTRUCT(BlueprintType)
struct FSDerivedAttributeBaseValues
{
    GENERATED_BODY()

    FSDerivedAttributeBaseValues()
        : BaseHitPoints(0)
        , BaseFortitudeSave(0)
        , BaseReflexSave(0)
        , BaseWillSave(0)
        , ArmorBonus(0)
        , ShieldBonus(0)
        , NaturalArmor(0)
        , DeflectionBonus(0)
        , DodgeBonus(0)
    {}

    // Base HP from class and level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|Base Values")
    int32 BaseHitPoints;

    // Base save bonuses from class and level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|Base Values")
    int32 BaseFortitudeSave;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|Base Values")
    int32 BaseReflexSave;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|Base Values")
    int32 BaseWillSave;

    // AC bonuses from various sources
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|AC Bonuses")
    int32 ArmorBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|AC Bonuses")
    int32 ShieldBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|AC Bonuses")
    int32 NaturalArmor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|AC Bonuses")
    int32 DeflectionBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes|AC Bonuses")
    int32 DodgeBonus;
};
