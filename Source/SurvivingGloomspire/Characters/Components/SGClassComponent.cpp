// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGClassComponent.h"
#include "SGCharacterClassData.h"

USGClassComponent::USGClassComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentXP = 0;
}

void USGClassComponent::BeginPlay()
{
    Super::BeginPlay();
    
    OwnerCharacter = GetOwner();
    
    // Initialize with level 1 in a default class if no levels exist
    if (ClassLevels.Num() == 0)
    {
        AddClassLevel(ESGClassType::Fighter);
    }
}

bool USGClassComponent::AddExperience(int32 Amount)
{
    if (Amount <= 0)
    {
        return false;
    }
    
    const int32 OldXP = CurrentXP;
    CurrentXP += Amount;
    
    // Notify that experience was gained
    OnExperienceGained.Broadcast(GetOwner(), Amount, CurrentXP);
    
    // Check for level up
    bool bLeveledUp = false;
    while (GetXPForNextLevel() > 0 && CurrentXP >= GetXPForNextLevel())
    {
        // Add a level in the highest class
        if (ClassLevels.Num() > 0)
        {
            AddClassLevel(ClassLevels[0].ClassType);
            bLeveledUp = true;
        }
        else
        {
            break;
        }
    }
    
    return bLeveledUp;
}

int32 USGClassComponent::GetXPForNextLevel() const
{
    return CalculateXPForLevel(GetCharacterLevel() + 1);
}

int32 USGClassComponent::GetCharacterLevel() const
{
    return GetTotalLevels();
}

bool USGClassComponent::AddClassLevel(ESGClassType ClassType, bool bAllowMulticlass)
{
    // Check if we can add a level in this class
    if (!bAllowMulticlass && ClassLevels.Num() > 0 && ClassLevels[0].ClassType != ClassType)
    {
        return false;
    }
    
    // Find or add the class level entry
    FSGCharacterClassLevel* ClassLevel = ClassLevels.FindByPredicate(
        [ClassType](const FSGCharacterClassLevel& Level) { return Level.ClassType == ClassType; });
    
    if (ClassLevel)
    {
        ClassLevel->Level++;
    }
    else
    {
        FSGCharacterClassLevel NewClassLevel;
        NewClassLevel.ClassType = ClassType;
        NewClassLevel.Level = 1;
        ClassLevels.Add(NewClassLevel);
    }
    
    // Apply features for the new level
    ApplyClassFeatures();
    
    // Notify about the level up
    OnLevelUp.Broadcast(GetOwner(), ClassType, GetCharacterLevel());
    
    return true;
}

int32 USGClassComponent::GetTotalLevels() const
{
    int32 TotalLevels = 0;
    for (const FSGCharacterClassLevel& ClassLevel : ClassLevels)
    {
        TotalLevels += ClassLevel.Level;
    }
    return TotalLevels;
}

int32 USGClassComponent::GetBaseAttackBonus() const
{
    // TODO: Implement actual BAB calculation based on class levels
    // For now, return a simple 1:1 BAB progression
    return GetTotalLevels();
}

int32 USGClassComponent::CalculateXPForLevel(int32 Level)
{
    if (Level <= 1)
    {
        return 0;
    }
    
    // Standard Pathfinder XP progression
    static const TArray<int32> XPTable = {
        0,       // Level 1
        2000,    // 2
        5000,    // 3
        9000,    // 4
        15000,   // 5
        23000,   // 6
        35000,   // 7
        51000,   // 8
        75000,   // 9
        105000,  // 10
        155000,  // 11
        220000,  // 12
        315000,  // 13
        445000,  // 14
        635000,  // 15
        890000,  // 16
        1300000, // 17
        1800000, // 18
        2550000, // 19
        3600000  // 20
    };
    
    if (Level <= XPTable.Num())
    {
        return XPTable[Level - 1];
    }
    
    // For levels beyond the table, add 1,000,000 XP per level
    return XPTable.Last() + (Level - XPTable.Num()) * 1000000;
}

void USGClassComponent::ApplyClassFeatures()
{
    // TODO: Implement feature application
    // This would apply all features from all class levels
}

void USGClassComponent::ApplyClassFeature(const FSGClassFeature& Feature)
{
    // TODO: Implement feature application logic
    // This would handle applying the actual game effects of the feature
}
