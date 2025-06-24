// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGFeatTypes.generated.h"

/**
 * Enumerates all feats available in the game.
 * Based on Pathfinder 1e feats.
 */
UENUM(BlueprintType)
enum class ESGFeatType : uint8
{
    // Combat Feats
    PowerAttack,
    WeaponFinesse,
    WeaponFocus,
    Dodge,
    Mobility,
    SpringAttack,
    
    // Metamagic Feats
    EmpowerSpell,
    MaximizeSpell,
    QuickenSpell,
    
    // Item Creation Feats
    CraftWondrousItem,
    CraftMagicArmsAndArmor,
    
    // General Feats
    Alertness,
    SkillFocus,
    Toughness,
    
    // Teamwork Feats
    Outflank,
    PreciseStrike,
    
    // Add MAX at the end for iteration
    MAX UMETA(Hidden)
};

/**
 * Types of prerequisites that can be required for a feat
 */
UENUM(BlueprintType)
enum class ESGFeatPrerequisiteType : uint8
{
    None,
    Level,
    Attribute,
    BaseAttackBonus,
    SkillRank,
    Feat,
    ClassLevel,
    CasterLevel,
    Race,
    Alignment,
    Size,
    Special
};
