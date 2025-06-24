// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGAttributeType.h"
#include "SGSkillType.generated.h"

/**
 * Enumerates all skills available in the game.
 * Based on Pathfinder 1e skill list.
 */
UENUM(BlueprintType)
enum class ESGSkillType : uint8
{
    // Physical Skills
    Acrobatics,
    Climb,
    EscapeArtist,
    Fly,
    Ride,
    Stealth,
    Swim,
    
    // Social Skills
    Bluff,
    Diplomacy,
    Disguise,
    HandleAnimal,
    Intimidate,
    Perform,
    
    // Knowledge Skills
    Appraise,
    Craft,
    KnowledgeArcana,
    KnowledgeDungeoneering,
    KnowledgeEngineering,
    KnowledgeGeography,
    KnowledgeHistory,
    KnowledgeLocal,
    KnowledgeNature,
    KnowledgeNobility,
    KnowledgePlanes,
    KnowledgeReligion,
    Linguistics,
    Profession,
    
    // Perception Skills
    DisableDevice,
    Perception,
    SenseMotive,
    SleightOfHand,
    
    // Other Skills
    Heal,
    Spellcraft,
    Survival,
    UseMagicDevice,
    
    // Add MAX at the end for iteration
    MAX UMETA(Hidden)
};

// Helper function to get skill name as string
static FString GetSkillName(ESGSkillType SkillType)
{
    const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/SurvivingGloomspire.ESGSkillType"), true);
    if (!EnumPtr) return FString("Unknown");
    
    // Convert enum to display name (e.g., "KnowledgeArcana" -> "Knowledge (Arcana)")
    FString DisplayName = EnumPtr->GetNameStringByValue(static_cast<int64>(SkillType));
    
    // Add spaces before capital letters and handle special cases
    DisplayName = DisplayName.Replace(TEXT("Knowledge"), TEXT("Knowledge ("), ESearchCase::CaseSensitive);
    DisplayName = DisplayName.Replace(TEXT("Arcana"), TEXT("Arcana)"), ESearchCase::CaseSensitive);
    DisplayName = DisplayName.Replace(TEXT("Dungeoneering"), TEXT("Dungeoneering)"), ESearchCase::CaseSensitive);
    // Add more knowledge skills as needed...
    
    return DisplayName;
}

// Helper function to get skill's key ability
static ESGAttributeType GetSkillKeyAbility(ESGSkillType SkillType)
{
    // Default to Intelligence for knowledge skills
    if (SkillType >= ESGSkillType::KnowledgeArcana && 
        SkillType <= ESGSkillType::KnowledgeReligion)
    {
        return ESGAttributeType::INT;
    }
    
    // Other skill to ability mappings
    switch (SkillType)
    {
        // Strength-based skills
        case ESGSkillType::Climb:
        case ESGSkillType::Swim:
            return ESGAttributeType::STR;
            
        // Dexterity-based skills
        case ESGSkillType::Acrobatics:
        case ESGSkillType::EscapeArtist:
        case ESGSkillType::Fly:
        case ESGSkillType::Ride:
        case ESGSkillType::Stealth:
        case ESGSkillType::SleightOfHand:
            return ESGAttributeType::DEX;
            
        // Constitution-based skills
        // None in standard Pathfinder
        
        // Intelligence-based skills
        case ESGSkillType::Appraise:
        case ESGSkillType::Craft:
        case ESGSkillType::KnowledgeArcana:
        case ESGSkillType::KnowledgeDungeoneering:
        case ESGSkillType::KnowledgeEngineering:
        case ESGSkillType::KnowledgeGeography:
        case ESGSkillType::KnowledgeHistory:
        case ESGSkillType::KnowledgeLocal:
        case ESGSkillType::KnowledgeNature:
        case ESGSkillType::KnowledgeNobility:
        case ESGSkillType::KnowledgePlanes:
        case ESGSkillType::KnowledgeReligion:
        case ESGSkillType::Linguistics:
        case ESGSkillType::Spellcraft:
            return ESGAttributeType::INT;
            
        // Wisdom-based skills
        case ESGSkillType::HandleAnimal:
        case ESGSkillType::Heal:
        case ESGSkillType::Perception:
        case ESGSkillType::Profession:
        case ESGSkillType::SenseMotive:
        case ESGSkillType::Survival:
            return ESGAttributeType::WIS;
            
        // Charisma-based skills
        case ESGSkillType::Bluff:
        case ESGSkillType::Diplomacy:
        case ESGSkillType::Disguise:
        case ESGSkillType::Intimidate:
        case ESGSkillType::Perform:
        case ESGSkillType::UseMagicDevice:
            return ESGAttributeType::CHA;
            
        default:
            return ESGAttributeType::INT; // Default to INT for any unhandled skills
    }
}
