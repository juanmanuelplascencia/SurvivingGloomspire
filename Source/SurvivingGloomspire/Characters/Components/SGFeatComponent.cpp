// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGFeatComponent.h"
#include "SGFeatData.h"
#include "SGFeatTypes.h"
#include "SGCharacterBase.h"

USGFeatComponent::USGFeatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USGFeatComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize if not already done
    if (!OwnerCharacter.IsValid() && GetOwner())
    {
        Initialize(Cast<ASGCharacterBase>(GetOwner()));
    }
}

void USGFeatComponent::Initialize(ASGCharacterBase* InOwnerCharacter)
{
    if (!InOwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("SGFeatComponent: Invalid owner character"));
        return;
    }

    OwnerCharacter = InOwnerCharacter;
    UE_LOG(LogTemp, Log, TEXT("SGFeatComponent: Initialized for %s"), *GetNameSafe(OwnerCharacter.Get()));
}

bool USGFeatComponent::HasFeat(ESGFeatType FeatType) const
{
    return FindFeatInstance(FeatType) != nullptr;
}

int32 USGFeatComponent::GetFeatStackCount(ESGFeatType FeatType) const
{
    if (const FSGFeatInstance* FeatInstance = FindFeatInstance(FeatType))
    {
        return FeatInstance->StackCount;
    }
    return 0;
}

bool USGFeatComponent::AddFeat(ESGFeatType FeatType, bool bCheckPrerequisites)
{
    if (!OwnerCharacter.IsValid())
    {
        return false;
    }

    // Check if we already have this feat
    if (FSGFeatInstance* ExistingFeat = FindFeatInstance(FeatType))
    {
        // If the feat can be taken multiple times, increase the stack count
        if (ExistingFeat->FeatData && ExistingFeat->FeatData->bCanTakeMultipleTimes)
        {
            if (ExistingFeat->StackCount < ExistingFeat->FeatData->MaxStackCount)
            {
                ExistingFeat->StackCount++;
                ExistingFeat->FeatData->ApplyBenefits(OwnerCharacter.Get(), ExistingFeat->StackCount);
                return true;
            }
        }
        return false;
    }

    // Check prerequisites if needed
    if (bCheckPrerequisites && !MeetsPrerequisites(FeatType))
    {
        return false;
    }

    // Create a new feat instance
    if (USGFeatData* FeatData = GetFeatData(FeatType))
    {
        FSGFeatInstance NewFeat(FeatType, FeatData);
        Feats.Add(NewFeat);
        
        // Apply the feat's benefits
        FeatData->ApplyBenefits(OwnerCharacter.Get());
        
        UE_LOG(LogTemp, Log, TEXT("Added feat %s to %s"), 
            *FeatData->DisplayName.ToString(), 
            *GetNameSafe(OwnerCharacter.Get()));
            
        return true;
    }
    
    return false;
}

bool USGFeatComponent::RemoveFeat(ESGFeatType FeatType)
{
    if (!OwnerCharacter.IsValid())
    {
        return false;
    }

    for (int32 i = 0; i < Feats.Num(); ++i)
    {
        if (Feats[i].FeatType == FeatType)
        {
            // Remove the feat's benefits before removing it
            if (Feats[i].FeatData)
            {
                Feats[i].FeatData->RemoveBenefits(OwnerCharacter.Get());
            }
            
            Feats.RemoveAt(i);
            return true;
        }
    }
    
    return false;
}

bool USGFeatComponent::MeetsPrerequisites(ESGFeatType FeatType) const
{
    if (!OwnerCharacter.IsValid())
    {
        return false;
    }

    if (USGFeatData* FeatData = GetFeatData(FeatType))
    {
        return FeatData->ArePrerequisitesMet(OwnerCharacter.Get());
    }
    
    return false;
}

int32 USGFeatComponent::GetFeatCountByCategory(FName Category) const
{
    int32 Count = 0;
    for (const FSGFeatInstance& Feat : Feats)
    {
        if (Feat.FeatData && Feat.FeatData->Category == Category)
        {
            Count++;
        }
    }
    return Count;
}

void USGFeatComponent::GetFeatsByCategory(FName Category, TArray<FSGFeatInstance>& OutFeats) const
{
    OutFeats.Reset();
    for (const FSGFeatInstance& Feat : Feats)
    {
        if (Feat.FeatData && Feat.FeatData->Category == Category)
        {
            OutFeats.Add(Feat);
        }
    }
}

USGFeatData* USGFeatComponent::GetFeatData(ESGFeatType FeatType) const
{
    // TODO: Implement feat data loading from data assets
    // For now, we'll create a default instance
    return NewObject<USGFeatData>();
}

FSGFeatInstance* USGFeatComponent::FindFeatInstance(ESGFeatType FeatType)
{
    for (FSGFeatInstance& Feat : Feats)
    {
        if (Feat.FeatType == FeatType)
        {
            return &Feat;
        }
    }
    return nullptr;
}

const FSGFeatInstance* USGFeatComponent::FindFeatInstance(ESGFeatType FeatType) const
{
    for (const FSGFeatInstance& Feat : Feats)
    {
        if (Feat.FeatType == FeatType)
        {
            return &Feat;
        }
    }
    return nullptr;
}
