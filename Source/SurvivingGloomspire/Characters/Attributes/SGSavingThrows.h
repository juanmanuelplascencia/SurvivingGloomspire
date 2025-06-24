// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGSavingThrows.generated.h"

/**
 * Enumerates the types of saving throws
 */
UENUM(BlueprintType)
enum class ESGSavingThrowType : uint8
{
    Fortitude   UMETA(DisplayName = "Fortitude"),
    Reflex     UMETA(DisplayName = "Reflex"),
    Will       UMETA(DisplayName = "Will")
};

/**
 * Structure containing saving throw data
 */
USTRUCT(BlueprintType)
struct FSGSavingThrowData
{
    GENERATED_BODY()

    FSGSavingThrowData()
        : BaseSave(0)
        , ResistanceBonus(0)
        , OtherBonus(0)
    {}

    // Base save from class and level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throw")
    int32 BaseSave;

    // Resistance bonus (from magic items, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throw")
    int32 ResistanceBonus;

    // Any other bonuses
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throw")
    int32 OtherBonus;

    /**
     * Calculates the total saving throw bonus
     * @param AbilityModifier The relevant ability modifier (CON for Fortitude, DEX for Reflex, WIS for Will)
     * @return Total saving throw bonus
     */
    int32 CalculateTotal(int32 AbilityModifier) const
    {
        return BaseSave + AbilityModifier + ResistanceBonus + OtherBonus;
    }
};

/**
 * Structure containing all saving throws
 */
USTRUCT(BlueprintType)
struct FSGSavingThrows
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throws")
    FSGSavingThrowData Fortitude;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throws")
    FSGSavingThrowData Reflex;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving Throws")
    FSGSavingThrowData Will;

    /**
     * Gets the specified saving throw data
     * @param Type The type of saving throw to get
     * @return Reference to the saving throw data
     */
    FSGSavingThrowData& GetSavingThrow(ESGSavingThrowType Type)
    {
        switch (Type)
        {
            case ESGSavingThrowType::Fortitude: return Fortitude;
            case ESGSavingThrowType::Reflex:    return Reflex;
            case ESGSavingThrowType::Will:      return Will;
            default:                            return Fortitude;
        }
    }

    /**
     * Gets the specified saving throw data (const version)
     * @param Type The type of saving throw to get
     * @return Const reference to the saving throw data
     */
    const FSGSavingThrowData& GetSavingThrow(ESGSavingThrowType Type) const
    {
        switch (Type)
        {
            case ESGSavingThrowType::Fortitude: return Fortitude;
            case ESGSavingThrowType::Reflex:    return Reflex;
            case ESGSavingThrowType::Will:      return Will;
            default:                            return Fortitude;
        }
    }
};
