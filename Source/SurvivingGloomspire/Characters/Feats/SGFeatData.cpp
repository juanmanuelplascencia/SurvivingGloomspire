// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGFeatData.h"
#include "SGCharacterBase.h"
#include "SGSkillComponent.h"

USGFeatData::USGFeatData()
    : FeatType(ESGFeatType::MAX)
    , bCanTakeMultipleTimes(false)
    , MaxStackCount(1)
{
}

FText USGFeatData::GetPrerequisitesText() const
{
    if (Prerequisites.Num() == 0)
    {
        return NSLOCTEXT("Feats", "NoPrerequisites", "None");
    }
    
    TArray<FText> PrereqTexts;
    for (const FSGFeatPrerequisite& Prereq : Prerequisites)
    {
        if (!Prereq.Description.IsEmpty())
        {
            PrereqTexts.Add(Prereq.Description);
        }
    }
    
    return FText::Join(FText::FromString(", "), PrereqTexts);
}

bool USGFeatData::ArePrerequisitesMet(ASGCharacterBase* Character) const
{
    if (!Character)
    {
        return false;
    }
    
    for (const FSGFeatPrerequisite& Prereq : Prerequisites)
    {
        if (!CheckPrerequisite(Prereq, Character))
        {
            return false;
        }
    }
    
    return true;
}

void USGFeatData::ApplyBenefits(ASGCharacterBase* Character, int32 StackCount) const
{
    if (!Character)
    {
        return;
    }
    
    // Apply each benefit
    for (const FSGFeatBenefit& Benefit : Benefits)
    {
        // TODO: Implement benefit application based on BenefitType
        // This will be expanded as we implement specific feat benefits
    }
}

void USGFeatData::RemoveBenefits(ASGCharacterBase* Character) const
{
    if (!Character)
    {
        return;
    }
    
    // Remove each benefit
    for (const FSGFeatBenefit& Benefit : Benefits)
    {
        // TODO: Implement benefit removal based on BenefitType
        // This will be expanded as we implement specific feat benefits
    }
}

bool USGFeatData::CheckPrerequisite(const FSGFeatPrerequisite& Prereq, ASGCharacterBase* Character) const
{
    if (!Character)
    {
        return false;
    }
    
    switch (Prereq.PrerequisiteType)
    {
        case ESGFeatPrerequisiteType::None:
            return true;
            
        case ESGFeatPrerequisiteType::Level:
            if (USGClassComponent* ClassComp = Character->GetClassComponent())
            {
                return ClassComp->GetTotalLevels() >= Prereq.Value;
            }
            return false;
            
        case ESGFeatPrerequisiteType::Attribute:
        {
            // Prereq.Subtype should be the attribute name (e.g., "Strength")
            // Prereq.Value is the minimum attribute score required
            // TODO: Implement attribute check
            return false;
        }
            
        case ESGFeatPrerequisiteType::BaseAttackBonus:
            // TODO: Implement BAB check
            return false;
            
        case ESGFeatPrerequisiteType::SkillRank:
        {
            // Prereq.Subtype should be the skill name
            // Prereq.Value is the minimum rank required
            if (USGSkillComponent* SkillComp = Character->GetSkillComponent())
            {
                // TODO: Look up skill type by name
                // int32 Ranks = SkillComp->GetSkillRanks(SkillType);
                // return Ranks >= Prereq.Value;
            }
            return false;
        }
            
        case ESGFeatPrerequisiteType::Feat:
        {
            // Prereq.Subtype should be the feat name
            // TODO: Implement feat check
            return false;
        }
            
        case ESGFeatPrerequisiteType::ClassLevel:
        {
            // Prereq.Subtype should be the class name
            // Prereq.Value is the minimum level required
            // TODO: Implement class level check
            return false;
        }
            
        case ESGFeatPrerequisiteType::CasterLevel:
            // TODO: Implement caster level check
            return false;
            
        case ESGFeatPrerequisiteType::Race:
        case ESGFeatPrerequisiteType::Alignment:
        case ESGFeatPrerequisiteType::Size:
        case ESGFeatPrerequisiteType::Special:
            // These will be implemented as needed
            return false;
            
        default:
            return false;
    }
}
