# Character Progression

## Overview
This document outlines the implementation of Pathfinder 1e's character progression system in Unreal Engine 5, including leveling, experience points, and class features.

## Core Progression Systems

### Experience Points (XP)
- Awarded for overcoming challenges
- XP requirements per level follow the Fast/Medium/Slow progression
- Tracked per character

### Leveling Up
1. Gain new hit points
2. Gain new class features/abilities
3. Increase base attack bonus
4. Improve saving throws
5. Gain skill points
6. Learn new spells (for casters)

## Implementation Details

### Data Structures

#### Character Class Data
```cpp
// Class data asset
UCLASS(Blueprintable)
class UPathfinderClassData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Class information
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText ClassName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText Description;
    
    // Hit Die type (d6, d8, d10, d12)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    int32 HitDie;
    
    // Base attack bonus progression (High/Medium/Low)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EAttackBonusProgression AttackBonusProgression;
    
    // Good saving throws
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TArray<ESavingThrowType> GoodSaves;
    
    // Class skills
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TArray<FName> ClassSkills;
    
    // Spells per day by level
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TArray<FSpellsPerDay> SpellsPerDay;
    
    // Class features by level
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TMap<int32, TArray<TSubclassOf<UClass>>> FeaturesByLevel;
};
```

#### Character Level Data
```cpp
// Character level information
USTRUCT(BlueprintType)
struct FCharacterLevelData
{
    GENERATED_BODY()

    // Class for this level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TSubclassOf<UPathfinderClassData> Class;
    
    // Level in this class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 Level = 1;
    
    // Hit points gained at this level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    int32 HitPoints = 0;
    
    // Skill ranks allocated at this level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TMap<FName, int32> SkillRanks;
    
    // Features gained at this level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<TSubclassOf<UClass>> Features;
    
    // Feats selected at this level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<TSubclassOf<UFeatData>> Feats;
};
```

### Experience and Leveling
```cpp
// Calculate experience needed for a level
int32 CalculateXPForLevel(int32 Level, EXPGrowthRate GrowthRate)
{
    switch (GrowthRate)
    {
    case EXPGrowthRate::Fast:
        return Level * 1000;
    case EXPGrowthRate::Medium:
        return Level * 1500;
    case EXPGrowthRate::Slow:
        return Level * 2000;
    default:
        return 0;
    }
}

// Add experience to character
void UCharacterProgressionComponent::AddExperience(int32 XP)
{
    if (XP <= 0) return;
    
    CurrentXP += XP;
    
    // Check for level up
    int32 NextLevelXP = CalculateXPForLevel(CharacterLevel + 1, GrowthRate);
    while (CurrentXP >= NextLevelXP && CharacterLevel < MaxLevel)
    {
        LevelUp();
        NextLevelXP = CalculateXPForLevel(CharacterLevel + 1, GrowthRate);
    }
}

// Level up the character
void UCharacterProgressionComponent::LevelUp()
{
    CharacterLevel++;
    
    // Create new level data
    FCharacterLevelData NewLevel;
    NewLevel.Class = CurrentClass;
    NewLevel.Level = CharacterLevel;
    
    // Calculate hit points
    int32 NewHP = FMath::RandRange(1, CurrentClass->HitDie);
    if (CharacterLevel == 1)
        NewHP += CurrentClass->HitDie; // Max at first level
    
    // Apply Constitution modifier
    NewHP += Attributes->GetAbilityModifier(EAbilityType::Constitution);
    NewLevel.HitPoints = FMath::Max(1, NewHP);
    
    // Add level to history
    LevelHistory.Add(NewLevel);
    
    // Apply level-up benefits
    ApplyLevelUpBenefits(NewLevel);
    
    // Notify UI and other systems
    OnLevelUp.Broadcast(CharacterLevel);
}
```

### Multiclassing
```cpp
// Add a new class level
bool UCharacterProgressionComponent::AddClassLevel(TSubclassOf<UPathfinderClassData> NewClass)
{
    if (!NewClass) return false;
    
    // Check multiclassing restrictions
    if (!CanMulticlassInto(NewClass))
        return false;
    
    // If already has levels in this class, increment level
    if (CurrentClass == NewClass)
    {
        return AddLevelToCurrentClass();
    }
    // Otherwise, start a new class
    else
    {
        return StartNewClass(NewClass);
    }
}

// Calculate total level in a specific class
int32 UCharacterProgressionComponent::GetClassLevel(TSubclassOf<UPathfinderClassData> ClassType) const
{
    int32 Total = 0;
    for (const FCharacterLevelData& Level : LevelHistory)
    {
        if (Level.Class == ClassType)
            Total++;
    }
    return Total;
}
```

### Feat System
```cpp
// Feat data asset
UCLASS(Blueprintable)
class UFeatData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Feat information
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    FText FeatName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    FText Description;
    
    // Prerequisites
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    TArray<FAttributeRequirement> AttributeRequirements;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    TArray<FFeatRequirement> FeatRequirements;
    
    // Gameplay effects when feat is taken
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    TArray<TSubclassOf<UGameplayEffect>> GrantedEffects;
    
    // Special abilities granted by this feat
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feat")
    TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};

// Check if character meets feat requirements
bool UCharacterProgressionComponent::CanTakeFeat(TSubclassOf<UFeatData> Feat) const
{
    if (!Feat) return false;
    
    UFeatData* FeatCDO = Feat.GetDefaultObject();
    if (!FeatCDO) return false;
    
    // Check attribute requirements
    for (const FAttributeRequirement& Req : FeatCDO->AttributeRequirements)
    {
        if (Attributes->GetAbilityScore(Req.Attribute) < Req.MinimumValue)
            return false;
    }
    
    // Check feat requirements
    for (const FFeatRequirement& Req : FeatCDO->FeatRequirements)
    {
        if (!HasFeat(Req.RequiredFeat))
            return false;
    }
    
    // Check other requirements (skill ranks, BAB, etc.)
    // ...
    
    return true;
}
```

## Related Systems
- [Attributes](attributes.md)
- [Skills](skills.md)
- [Spellcasting](spellcasting.md)
- [Combat Mechanics](combat_mechanics.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
