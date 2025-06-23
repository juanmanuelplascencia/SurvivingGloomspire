# Spellcasting System

## Overview
This document details the implementation of Pathfinder 1e's spellcasting system in Unreal Engine 5, including spell preparation, casting mechanics, and magic item creation.

## Core Concepts

### Spell Components
- **Verbal (V)**: Requires clear speech
- **Somatic (S)**: Requires hand gestures
- **Material (M)**: Requires material components
- **Focus (F)**: Requires a focus component
- **Divine Focus (DF)**: Requires a holy symbol

### Spell Levels
- 0-9 for arcane/divine spells
- 1-6 for certain class spell lists

### Casting Time
- Standard Action (most spells)
- Full-Round Action
- Swift/Immediate Action
- 1 Round or more (rituals)

## Implementation Details

### Data Structures

#### Spell Definition
```cpp
// Spell school enumeration
UENUM(BlueprintType)
enum class ESpellSchool : uint8
{
    Abjuration,
    Conjuration,
    Divination,
    Enchantment,
    Evocation,
    Illusion,
    Necromancy,
    Transmutation,
    Universal
};

// Spell descriptor flags
UENUM(BlueprintType)
enum class ESpellDescriptor : uint8
{
    None = 0,
    Acid = 1 << 0,
    Air = 1 << 1,
    Chaotic = 1 << 2,
    Cold = 1 << 3,
    Darkness = 1 << 4,
    Death = 1 << 5,
    // ... other descriptors
};
ENUM_CLASS_FLAGS(ESpellDescriptor);

// Spell component requirements
USTRUCT(BlueprintType)
struct FSpellComponents
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
    bool bVerbal = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
    bool bSomatic = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
    FString MaterialComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
    FString FocusComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
    bool bDivineFocus = false;
};

// Spell data asset
UCLASS(Blueprintable)
class USpellData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Basic spell info
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FText SpellName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    ESpellSchool School;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FSpellDescriptor Descriptors;
    
    // Spell level for different classes
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    TMap<TSubclassOf<UClass>, int32> ClassLevels;
    
    // Casting requirements
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FSpellComponents Components;
    
    // Range, duration, and area
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    float Range;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FString Duration;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    FString Area;
    
    // Saving throw information
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    bool bSavingThrowNegates = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    bool bSpellResistance = false;
    
    // Gameplay ability reference
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell")
    TSubclassOf<UGameplayAbility> GameplayAbility;
};
```

### Spellbook Management
```cpp
// Character's known/prepared spells
UCLASS()
class USpellbookComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Get spells known for a class
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    TArray<USpellData*> GetKnownSpells(TSubclassOf<UClass> ForClass) const;

    // Get spells prepared for a class
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    TArray<USpellData*> GetPreparedSpells(TSubclassOf<UClass> ForClass) const;

    // Prepare a spell
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    bool PrepareSpell(USpellData* Spell, int32 SlotLevel);

    // Cast a prepared spell
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    bool CastPreparedSpell(USpellData* Spell, const FGameplayEventData& Payload);

    // Get spells per day
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    int32 GetSpellsPerDay(TSubclassOf<UClass> ForClass, int32 Level) const;

    // Get caster level
    UFUNCTION(BlueprintCallable, Category = "Spellbook")
    int32 GetCasterLevel(TSubclassOf<UClass> ForClass) const;
};
```

## Spell Effects

### Basic Spell Effects
- **Damage**: Hit point reduction
- **Healing**: Hit point restoration
- **Buff/Debuff**: Temporary modifiers to attributes/skills
- **Summoning**: Create creatures/objects
- **Transmutation**: Change properties of objects/creatures
- **Divination**: Gain information

### Implementing Spell Effects
```cpp
// Base class for spell effects
UCLASS(Abstract, Blueprintable)
class USpellEffect : public UObject
{
    GENERATED_BODY()

public:
    // Apply the spell effect
    UFUNCTION(BlueprintNativeEvent, Category = "Spell")
    void Apply(const FSpellCastParameters& Parameters);

    // Can this effect be applied?
    UFUNCTION(BlueprintNativeEvent, Category = "Spell")
    bool CanApply(const FSpellCastParameters& Parameters) const;

    // Get the spell's save DC
    UFUNCTION(BlueprintCallable, Category = "Spell")
    int32 CalculateSaveDC(const AActor* Caster, USpellData* Spell) const;
};
```

## Spell Resistance
```cpp
// Check spell resistance
bool CheckSpellResistance(const AActor* Caster, const AActor* Target, USpellData* Spell)
{
    // Get target's spell resistance
    ICombatInterface* CombatTarget = Cast<ICombatInterface>(Target);
    if (!CombatTarget) return false;
    
    int32 SR = CombatTarget->GetSpellResistance();
    if (SR <= 0) return true; // No SR or SR 0 = no resistance
    
    // Calculate caster level check
    int32 CasterLevel = Caster->FindComponentByClass<USpellbookComponent>()->GetCasterLevel(Spell->GetClass());
    int32 Check = FMath::RandRange(1, 20) + CasterLevel;
    
    return Check >= SR;
}
```

## Related Systems
- [Attributes](attributes.md)
- [Combat Mechanics](combat_mechanics.md)
- [Character Progression](character_progression.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
