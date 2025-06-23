# Skills System

## Overview
This document outlines the implementation of Pathfinder 1e's skill system in Unreal Engine 5, including skill checks, class skills, and skill synergies.

## Core Concepts

### Skill Check Formula
```
Skill Check Result = d20 + Ranks + Ability Modifier + Class Skill Bonus (if trained) + Miscellaneous Modifiers
```

### Skill List

#### Physical Skills
- **Acrobatics** (DEX)
- **Climb** (STR)
- **Escape Artist** (DEX)
- **Fly** (DEX)
- **Ride** (DEX)
- **Stealth** (DEX)
- **Swim** (STR)

#### Social Skills
- **Bluff** (CHA)
- **Diplomacy** (CHA)
- **Handle Animal** (CHA)
- **Intimidate** (CHA)
- **Perform** (CHA)
- **Use Magic Device** (CHA)

#### Knowledge Skills
- **Appraise** (INT)
- **Craft** (INT)
- **Knowledge (Arcana, Dungeoneering, etc.)** (INT)
- **Linguistics** (INT)
- **Profession** (WIS)
- **Spellcraft** (INT)

#### Other Skills
- **Disable Device** (DEX)
- **Disguise** (CHA)
- **Heal** (WIS)
- **Perception** (WIS)
- **Sense Motive** (WIS)
- **Sleight of Hand** (DEX)
- **Survival** (WIS)

## Implementation Details

### Data Structures

#### Skill Definition
```cpp
// Skill definition structure
USTRUCT(BlueprintType)
struct FPathfinderSkill
{
    GENERATED_BODY()

    // Skill name
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
    FName SkillName;

    // Associated ability score
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
    EAbilityType KeyAbility;

    // Whether this skill can be used untrained
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
    bool bCanUseUntrained;

    // Armor check penalty applies
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
    bool bArmorCheckPenalty;
};
```

#### Character Skill Data
```cpp
// Character's skill data
USTRUCT(BlueprintType)
struct FCharacterSkill
{
    GENERATED_BODY()

    // Number of ranks in this skill
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
    int32 Ranks;

    // Whether this is a class skill
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
    bool bIsClassSkill;

    // Get total skill bonus
    UFUNCTION(BlueprintCallable, Category = "Skills")
    int32 GetTotalBonus(const UPathfinderAttributeSet* Attributes) const
    {
        int32 Bonus = Ranks;
        
        // Add class skill bonus if trained
        if (Ranks > 0 && bIsClassSkill)
            Bonus += 3; // +3 class skill bonus
            
        // Add ability modifier
        Bonus += Attributes->GetAbilityModifier(GetKeyAbility());
        
        // Add other modifiers (feats, equipment, etc.)
        // ...
        
        return Bonus;
    }
};
```

### Skill Check Execution

```cpp
// Perform a skill check
UFUNCTION(BlueprintCallable, Category = "Skills")
static bool PerformSkillCheck(
    const UObject* WorldContextObject, 
    const FName& SkillName, 
    AActor* Instigator, 
    int32 DifficultyClass)
{
    // Get character's skill component
    USkillComponent* SkillComp = Instigator->FindComponentByClass<USkillComponent>();
    if (!SkillComp) return false;

    // Get skill data
    FCharacterSkill* SkillData = SkillComp->GetSkillData(SkillName);
    if (!SkillData) return false;

    // Roll d20
    int32 Roll = FMath::RandRange(1, 20);
    
    // Get total bonus
    int32 TotalBonus = SkillData->GetTotalBonus(SkillComp->GetAttributes());
    
    // Calculate result
    int32 Result = Roll + TotalBonus;
    
    // Handle natural 1 (automatic failure) and 20 (automatic success)
    if (Roll == 1) return false;
    if (Roll == 20) return true;
    
    return Result >= DifficultyClass;
}
```

## Class Skills
- Each class has specific class skills
- +3 bonus when trained in a class skill
- Maximum ranks = character level + 3 for class skills
- Maximum ranks = (character level + 3)/2 for cross-class skills

## Synergy Bonuses
Certain skills provide bonuses to other skills when trained:
- **Knowledge (Arcana)** +5 to Spellcraft for magic item identification
- **Knowledge (Dungeoneering)** +2 on Survival checks underground
- **Knowledge (Nature)** +2 on Survival checks in aboveground natural environments
- **Knowledge (Planes)** +2 on Survival checks while on other planes
- **Knowledge (Religion)** +2 on channel energy DCs to harm undead
- **Knowledge (The Planes)** +2 on Survival checks on other planes
- **Linguistics** +1 per rank on Decipher Script checks
- **Ride** +2 on Handle Animal checks for warhorses and warponies
- **Spellcraft** +2 to Use Magic Device checks involving scrolls

## Skill Unlocks
- Certain feats and class features unlock additional uses for skills
- Example: Skill Focus, Signature Skill

## Related Systems
- [Attributes](attributes.md)
- [Character Progression](character_progression.md)
- [Combat Mechanics](combat_mechanics.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
