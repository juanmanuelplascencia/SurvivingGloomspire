// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGArmorClass.generated.h"

/**
 * Structure containing armor class related attributes
 */
USTRUCT(BlueprintType)
struct FSGArmorClass
{
    GENERATED_BODY()

    FSGArmorClass()
        : Base(10) // Base AC is 10 in Pathfinder
        , Touch(10)
        , FlatFooted(10)
        , ArmorBonus(0)
        , ShieldBonus(0)
        , NaturalArmor(0)
        , DeflectionBonus(0)
        , DodgeBonus(0)
    {}

    // Base AC values
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|AC")
    int32 Base;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|AC")
    int32 Touch;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|AC")
    int32 FlatFooted;

    // AC bonuses from various sources
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|AC|Bonuses")
    int32 ArmorBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|AC|Bonuses")
    int32 ShieldBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|AC|Bonuses")
    int32 NaturalArmor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|AC|Bonuses")
    int32 DeflectionBonus;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|AC|Bonuses")
    int32 DodgeBonus;

    /**
     * Calculates the total AC based on all bonuses
     * @param DexterityModifier Character's Dexterity modifier
     * @param SizeModifier Character's size modifier
     * @param OtherModifiers Any other modifiers to AC
     * @return Total AC value
     */
    int32 CalculateTotalAC(int32 DexterityModifier, int32 SizeModifier = 0, int32 OtherModifiers = 0) const
    {
        // Base 10 + armor + shield + Dex modifier + size modifier + natural armor + deflection + other modifiers
        // Dodge bonuses stack with themselves and everything else
        return Base + 
               ArmorBonus + 
               ShieldBonus + 
               FMath::Max(0, DexterityModifier) + // Max Dex bonus from armor would be applied here
               SizeModifier + 
               NaturalArmor + 
               DeflectionBonus + 
               DodgeBonus + 
               OtherModifiers;
    }

    /**
     * Calculates touch AC (ignores armor, shield, and natural armor bonuses)
     * @param DexterityModifier Character's Dexterity modifier
     * @param SizeModifier Character's size modifier
     * @param OtherModifiers Any other modifiers to AC
     * @return Touch AC value
     */
    int32 CalculateTouchAC(int32 DexterityModifier, int32 SizeModifier = 0, int32 OtherModifiers = 0) const
    {
        // Base 10 + Dex modifier + size modifier + deflection + dodge + other modifiers
        return Base + 
               FMath::Max(0, DexterityModifier) + 
               SizeModifier + 
               DeflectionBonus + 
               DodgeBonus + 
               OtherModifiers;
    }

    /**
     * Calculates flat-footed AC (ignores Dexterity and dodge bonuses)
     * @param SizeModifier Character's size modifier
     * @param OtherModifiers Any other modifiers to AC
     * @return Flat-footed AC value
     */
    int32 CalculateFlatFootedAC(int32 SizeModifier = 0, int32 OtherModifiers = 0) const
    {
        // Base 10 + armor + shield + size modifier + natural armor + deflection + other modifiers
        return Base + 
               ArmorBonus + 
               ShieldBonus + 
               SizeModifier + 
               NaturalArmor + 
               DeflectionBonus + 
               OtherModifiers;
    }
};
