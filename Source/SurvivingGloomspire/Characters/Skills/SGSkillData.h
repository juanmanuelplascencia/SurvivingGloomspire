// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGSkillType.h"
#include "SGSkillData.generated.h"

/**
 * Represents the data for a single skill
 */
USTRUCT(BlueprintType)
struct FSGSkillData
{
    GENERATED_BODY()

    // Default constructor
    FSGSkillData()
        : Ranks(0)
        , ClassSkill(false)
        , TrainedOnly(false)
        , ArmorCheckPenalty(0.0f)
    {
    }

    FSGSkillData(bool bIsClassSkill, bool bIsTrainedOnly = false)
        : Ranks(0)
        , ClassSkill(bIsClassSkill)
        , TrainedOnly(bIsTrainedOnly)
        , ArmorCheckPenalty(0.0f)
    {
    }

    /** Number of ranks in this skill */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 Ranks;

    /** Whether this is a class skill */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool ClassSkill;

    /** Whether this skill can only be used with at least 1 rank */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool TrainedOnly;

    /** Armor check penalty to apply to this skill */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float ArmorCheckPenalty;

    /**
     * Calculate the total bonus for this skill
     * @param AbilityModifier The relevant ability modifier (e.g., DEX for Acrobatics)
     * @param MiscModifier Any miscellaneous modifiers to apply
     * @return The total skill bonus
     */
    int32 CalculateTotalBonus(int32 AbilityModifier, int32 MiscModifier = 0) const
    {
        int32 Total = Ranks + AbilityModifier + MiscModifier;
        
        // Apply class skill bonus if applicable
        if (ClassSkill && Ranks > 0)
        {
            Total += 3; // +3 bonus for class skills with at least 1 rank
        }
        
        // Apply armor check penalty
        Total += FMath::FloorToInt(ArmorCheckPenalty);
        
        return Total;
    }

    /**
     * Check if this skill can be used based on training requirements
     */
    bool CanUseSkill() const
    {
        return !TrainedOnly || Ranks > 0;
    }
};

/**
 * Map of skill type to skill data
 */
USTRUCT(BlueprintType)
struct FSGSkillContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
    TMap<ESGSkillType, FSGSkillData> Skills;

    /**
     * Get a skill by type, creating it if it doesn't exist
     */
    FSGSkillData& GetOrAddSkill(ESGSkillType SkillType, bool bIsClassSkill = false, bool bIsTrainedOnly = false)
    {
        if (!Skills.Contains(SkillType))
        {
            Skills.Add(SkillType, FSGSkillData(bIsClassSkill, bIsTrainedOnly));
        }
        return Skills[SkillType];
    }

    /**
     * Get a skill by type (const version)
     */
    const FSGSkillData* GetSkill(ESGSkillType SkillType) const
    {
        return Skills.Find(SkillType);
    }

    /**
     * Get the total bonus for a skill
     */
    int32 GetSkillBonus(ESGSkillType SkillType, int32 AbilityModifier, int32 MiscModifier = 0) const
    {
        const FSGSkillData* SkillData = GetSkill(SkillType);
        return SkillData ? SkillData->CalculateTotalBonus(AbilityModifier, MiscModifier) : 0;
    }
};
