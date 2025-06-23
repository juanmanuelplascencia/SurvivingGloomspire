# Attributes System

## Overview
Pathfinder 1e's attribute system is built around six core ability scores that form the foundation of a character's capabilities. This document details their implementation in Unreal Engine 5.

## Core Attributes

### Attribute List
1. **Strength (STR)**
   - Affects melee attack rolls, damage, and physical skills
   - Used for: Climb, Swim, Combat Maneuver Bonus (CMB)

2. **Dexterity (DEX)**
   - Influences ranged attack rolls, Armor Class, and Reflex saves
   - Used for: Acrobatics, Stealth, Initiative

3. **Constitution (CON)**
   - Determines hit points and Fortitude saves
   - Used for: Concentration checks

4. **Intelligence (INT)**
   - Affects skill points and knowledge-based skills
   - Used for: Arcane spellcasting (Wizard)

5. **Wisdom (WIS)**
   - Influences Will saves and perception abilities
   - Used for: Divine spellcasting (Cleric, Druid)

6. **Charisma (CHA)**
   - Affects social interactions and certain class abilities
   - Used for: Sorcerer and Paladin spellcasting

## Implementation Details

### Data Structure
```cpp
// Core attribute structure
USTRUCT(BlueprintType)
struct FPathfinderAttribute
{
    GENERATED_BODY()

    // Base value (10 + racial/level modifiers)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    int32 BaseValue;

    // Current value (base + temporary modifiers)
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    int32 CurrentValue;

    // Calculate modifier: (Value - 10) / 2 (rounded down)
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    int32 GetModifier() const { return FMath::FloorToInt((CurrentValue - 10) / 2.0f); }
};

// Attribute set for the character
UCLASS()
class SURVIVINGGLOOMSPIRE_API UPathfinderAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPathfinderAttributeSet();

    // Core attributes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FPathfinderAttribute Strength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FPathfinderAttribute Dexterity;
    
    // ... other attributes ...
};
```

### Modifier System
Attributes can be modified by various effects:
- **Enhancement**: Magical bonuses (e.g., +2 Strength belt)
- **Inherent**: Permanent increases (e.g., level-up bonuses)
- **Morale**: Temporary boosts from spells/effects
- **Circumstance**: Situational bonuses/penalties

### Implementation Notes
1. **Replication**: All attributes are replicated to clients
2. **Prediction**: Critical attributes support client-side prediction
3. **Events**: Attribute changes trigger events for UI updates

## Usage Examples

### Getting an Attribute Modifier
```cpp
// Get Strength modifier
int32 StrengthMod = Character->GetAttributeSet()->Strength.GetModifier();

// Use in melee attack calculation
int32 AttackRoll = FMath::RandRange(1, 20) + StrengthMod + BaseAttackBonus;
```

### Applying Temporary Bonuses
```cpp
// Apply a +2 enhancement bonus to Strength
FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StrengthBuffEffect, 1, EffectContext);
if (SpecHandle.IsValid())
{
    SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Set.ByCaller.Strength"), 2.0f);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
```

## Data Tables
Attributes are defined in `DT_AttributeDefaults` with base values for different character types.

## Related Systems
- [Combat Mechanics](combat_mechanics.md)
- [Character Progression](character_progression.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
