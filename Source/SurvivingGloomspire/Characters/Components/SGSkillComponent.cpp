// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGSkillComponent.h"
#include "SGCharacterBase.h"
#include "SGSkillType.h"
#include "Math/UnrealMathUtility.h"

USGSkillComponent::USGSkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USGSkillComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize if not already done
    if (!OwnerCharacter.IsValid() && GetOwner())
    {
        Initialize(Cast<ASGCharacterBase>(GetOwner()));
    }
}

void USGSkillComponent::InitializeComponent()
{
    Super::InitializeComponent();
    
    // Initialize if not already done
    if (!OwnerCharacter.IsValid() && GetOwner())
    {
        Initialize(Cast<ASGCharacterBase>(GetOwner()));
    }
}

void USGSkillComponent::Initialize(ASGCharacterBase* InOwnerCharacter)
{
    if (!InOwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("SGSkillComponent: Invalid owner character"));
        return;
    }

    OwnerCharacter = InOwnerCharacter;
    
    // Initialize skills if empty
    if (Skills.Skills.Num() == 0)
    {
        // Add all skills with default values
        for (uint8 i = 0; i < static_cast<uint8>(ESGSkillType::MAX); ++i)
        {
            ESGSkillType SkillType = static_cast<ESGSkillType>(i);
            Skills.GetOrAddSkill(SkillType, false, false);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("SGSkillComponent: Initialized for %s"), *GetNameSafe(OwnerCharacter.Get()));
}

int32 USGSkillComponent::AddSkillRanks(ESGSkillType SkillType, int32 RanksToAdd)
{
    if (!OwnerCharacter.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SGSkillComponent: No owner character"));
        return 0;
    }

    // Get or add the skill
    FSGSkillData& SkillData = Skills.GetOrAddSkill(SkillType);
    
    // Update ranks, ensuring they don't go below 0
    const int32 NewRanks = FMath::Max(0, SkillData.Ranks + RanksToAdd);
    const int32 ActualRanksAdded = NewRanks - SkillData.Ranks;
    SkillData.Ranks = NewRanks;
    
    UE_LOG(LogTemp, Log, TEXT("SGSkillComponent: Added %d ranks to %s for %s (total: %d)"),
        RanksToAdd, *GetSkillDisplayName(SkillType), *GetNameSafe(OwnerCharacter.Get()), SkillData.Ranks);
    
    return SkillData.Ranks;
}

void USGSkillComponent::SetClassSkill(ESGSkillType SkillType, bool bIsClassSkill)
{
    if (!OwnerCharacter.IsValid())
    {
        return;
    }

    FSGSkillData& SkillData = Skills.GetOrAddSkill(SkillType);
    SkillData.ClassSkill = bIsClassSkill;
    
    UE_LOG(LogTemp, Log, TEXT("SGSkillComponent: Set %s as %s class skill for %s"),
        *GetSkillDisplayName(SkillType), bIsClassSkill ? TEXT("a") : TEXT("not a"), *GetNameSafe(OwnerCharacter.Get()));
}

int32 USGSkillComponent::GetSkillBonus(ESGSkillType SkillType) const
{
    if (!OwnerCharacter.IsValid())
    {
        return 0;
    }

    const int32 AbilityMod = GetAbilityModifierForSkill(SkillType);
    const int32 MiscMod = GetMiscModifiersForSkill(SkillType);
    
    if (const FSGSkillData* SkillData = Skills.GetSkill(SkillType))
    {
        return SkillData->CalculateTotalBonus(AbilityMod, MiscMod);
    }
    
    return 0;
}

void USGSkillComponent::PerformSkillCheck(ESGSkillType SkillType, int32 DifficultyClass, int32 Modifier, bool& bOutSuccess, int32& OutRollResult, int32& OutDC) const
{
    if (!OwnerCharacter.IsValid())
    {
        bOutSuccess = false;
        OutRollResult = 0;
        OutDC = DifficultyClass;
        return;
    }

    // Roll a d20
    OutRollResult = FMath::RandRange(1, 20);
    OutDC = DifficultyClass;
    
    // Get the skill bonus
    const int32 SkillBonus = GetSkillBonus(SkillType) + Modifier;
    
    // Check if the roll plus bonus meets or exceeds the DC
    bOutSuccess = (OutRollResult + SkillBonus) >= OutDC;
    
    UE_LOG(LogTemp, Log, TEXT("SGSkillComponent: %s rolled %d + %d (bonus) vs DC %d - %s"),
        *GetSkillDisplayName(SkillType), OutRollResult, SkillBonus, OutDC,
        bOutSuccess ? TEXT("Success") : TEXT("Failure"));
}

int32 USGSkillComponent::GetSkillRanks(ESGSkillType SkillType) const
{
    if (const FSGSkillData* SkillData = Skills.GetSkill(SkillType))
    {
        return SkillData->Ranks;
    }
    return 0;
}

bool USGSkillComponent::IsClassSkill(ESGSkillType SkillType) const
{
    if (const FSGSkillData* SkillData = Skills.GetSkill(SkillType))
    {
        return SkillData->ClassSkill;
    }
    return false;
}

bool USGSkillComponent::CanUseSkill(ESGSkillType SkillType) const
{
    if (const FSGSkillData* SkillData = Skills.GetSkill(SkillType))
    {
        return SkillData->CanUseSkill();
    }
    return false;
}

ESGAttributeType USGSkillComponent::GetKeyAbilityForSkill(ESGSkillType SkillType)
{
    return GetSkillKeyAbility(SkillType);
}

FString USGSkillComponent::GetSkillDisplayName(ESGSkillType SkillType)
{
    return GetSkillName(SkillType);
}

int32 USGSkillComponent::GetAbilityModifierForSkill(ESGSkillType SkillType) const
{
    if (!OwnerCharacter.IsValid())
    {
        return 0;
    }

    const ESGAttributeType AbilityType = GetKeyAbilityForSkill(SkillType);
    return OwnerCharacter->GetAttributeModifier(AbilityType);
}

int32 USGSkillComponent::GetMiscModifiersForSkill(ESGSkillType SkillType) const
{
    // TODO: Implement this to handle various skill bonuses from feats, items, etc.
    return 0;
}
