# Combat Mechanics

## Overview
This document details the implementation of Pathfinder 1e's turn-based combat system in Unreal Engine 5, including attack resolution, damage calculation, and action economy.

## Core Combat Loop

1. **Initiative**
   - Roll d20 + Dexterity modifier + other bonuses
   - Determines turn order
   - Tracks rounds and turns

2. **Turn Structure**
   - Standard Action
   - Move Action
   - Swift/Immediate Actions
   - Free Actions
   - Attacks of Opportunity

## Attack Resolution

### Attack Roll Formula
```
Attack Roll = d20 + Base Attack Bonus (BAB) + Ability Modifier + Size Modifier + Miscellaneous Modifiers
```

### Attack Types
- **Melee Attacks**: Use Strength modifier
- **Ranged Attacks**: Use Dexterity modifier
- **Touch Attacks**: Target Touch AC
- **Combat Maneuvers**: Use CMB vs CMD

### Critical Hits
- Confirm critical threat with second attack roll
- Multiply damage on success
- Critical range varies by weapon

## Damage System

### Damage Calculation
```
Base Damage = Weapon Damage + Strength Modifier + Enhancement Bonus + Other Modifiers
```

### Damage Types
- Slashing
- Piercing
- Blunt
- Energy (Fire, Cold, Electricity, etc.)
- Positive/Negative Energy
- Force

### Resistance and Vulnerability
- Damage Reduction (DR)
- Energy Resistance
- Immunities

## Armor Class (AC)

### AC Calculation
```
AC = 10 + Armor Bonus + Shield Bonus + Dexterity Modifier (max Dex) + Size Modifier + Natural Armor + Deflection + Dodge - Penalties
```

### AC Types
- Normal AC
- Touch AC
- Flat-Footed AC

## Implementation Details

### Data Structures
```cpp
// Combat data for a character
USTRUCT(BlueprintType)
struct FCombatStats
{
    GENERATED_BODY()

    // Base Attack Bonus
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 BaseAttackBonus;

    // Combat Maneuver Bonus (CMB)
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    int32 GetCombatManeuverBonus() const 
    { 
        return BaseAttackBonus + StrengthModifier + SizeModifier; 
    }

    // Combat Maneuver Defense (CMD)
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    int32 GetCombatManeuverDefense() const
    {
        return 10 + BaseAttackBonus + StrengthModifier + DexterityModifier + SizeModifier;
    }
};
```

### Combat Actions
```cpp
// Base class for combat actions
UCLASS(Blueprintable, Abstract)
class UCombatAction : public UObject
{
    GENERATED_BODY()

public:
    // Execute the combat action
    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    void Execute(AActor* Instigator, AActor* Target);

    // Can this action be performed?
    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    bool CanExecute(const AActor* Instigator) const;

    // Action type (Standard, Move, Swift, etc.)
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    ECombatActionType ActionType;
};
```

## Combat Flow

1. **Pre-combat**
   - Roll initiative
   - Setup combat grid/positions

2. **Combat Round**
   - Process turns in initiative order
   - Resolve actions
   - Apply effects

3. **Post-combat**
   - Cleanup temporary effects
   - Award experience
   - Handle loot

## Special Combat Rules

### Attacks of Opportunity
- Triggered by specific actions
- Use immediate action
- Limited to one per trigger unless otherwise specified

### Flanking
- +2 attack bonus when flanking
- Requires positioning on opposite sides

### Cover and Concealment
- Cover bonuses to AC
- Concealment miss chance
- Varying degrees (partial, improved, etc.)

## Related Systems
- [Attributes](attributes.md)
- [Skills](skills.md)
- [Spellcasting](spellcasting.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
