# Data Structures

## Overview
This document outlines the core data structures used throughout the Pathfinder 1e implementation in Unreal Engine 5. These structures are designed for flexibility, performance, and ease of use in both C++ and Blueprints.

## Core Data Types

### Basic Types

#### Gameplay Tags
```cpp
// Gameplay tag container for flexible categorization
USTRUCT(BlueprintType)
struct FGameplayTagContainer
{
    GENERATED_BODY()

    // Add a tag to the container
    void AddTag(FGameplayTag Tag);
    
    // Remove a tag from the container
    void RemoveTag(FGameplayTag Tag);
    
    // Check if container has a specific tag
    bool HasTag(FGameplayTag Tag) const;
    
    // Check if container has all tags in another container
    bool HasAll(const FGameplayTagContainer& Other) const;
    
    // Check if container has any tags in another container
    bool HasAny(const FGameplayTagContainer& Other) const;
};
```

#### Gameplay Effects
```cpp
// Base structure for game effects
USTRUCT(BlueprintType)
struct FGameplayEffectSpec
{
    GENERATED_BODY()

    // Effect definition
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UGameplayEffect* Effect;
    
    // Source of the effect
    UPROPERTY(BlueprintReadOnly, Category = "Effect")
    AActor* Instigator = nullptr;
    
    // Target of the effect
    UPROPERTY(BlueprintReadOnly, Category = "Effect")
    AActor* Target = nullptr;
    
    // Effect level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    int32 Level = 1;
    
    // Duration in seconds (0 for instant)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Duration = 0.0f;
    
    // Apply the effect to a target
    UFUNCTION(BlueprintCallable, Category = "Effect")
    void ApplyToTarget(AActor* TargetActor);
};
```

## Character Data

### Attribute Set
```cpp
// Container for character attributes
UCLASS()
class UAttributeSet : public UObject
{
    GENERATED_BODY()

public:
    // Core attributes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Health;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float MaxHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Mana;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float MaxMana;
    
    // ... other attributes
    
    // Get attribute value by name
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetAttributeValue(FName AttributeName) const;
    
    // Set attribute value by name
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void SetAttributeValue(FName AttributeName, float NewValue);
};
```

### Character Class
```cpp
// Character class definition
USTRUCT(BlueprintType)
struct FCharacterClassInfo
{
    GENERATED_BODY()

    // Class name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FText ClassName;
    
    // Class description
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FText Description;
    
    // Hit die type (d6, d8, d10, d12)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    int32 HitDie;
    
    // Base attack bonus progression
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    EAttackBonusProgression AttackBonusProgression;
    
    // Good saving throws
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TArray<ESavingThrowType> GoodSaves;
    
    // Class skills
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TArray<FName> ClassSkills;
    
    // Spells per day by level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TArray<FSpellsPerDay> SpellsPerDay;
};
```

## Combat Data

### Attack Data
```cpp
// Data for an attack
USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

    // Attack roll result
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 AttackRoll;
    
    // Damage roll result
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 DamageRoll;
    
    // Critical threat range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CriticalThreatRange;
    
    // Critical multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CriticalMultiplier;
    
    // Attack type (melee, ranged, touch, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EAttackType AttackType;
    
    // Damage type (slashing, piercing, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EDamageType DamageType;
    
    // Special effects applied by this attack
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<FGameplayEffectSpec> Effects;
};
```

### Damage Data
```cpp
// Data for damage calculation
USTRUCT(BlueprintType)
struct FDamageData
{
    GENERATED_BODY()

    // Base damage amount
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float BaseDamage;
    
    // Damage type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EDamageType DamageType;
    
    // Damage source
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    AActor* DamageCauser = nullptr;
    
    // Instigator (who caused the damage)
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    AController* Instigator = nullptr;
    
    // Critical hit data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bIsCriticalHit = false;
    
    // Critical hit multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float CriticalMultiplier = 2.0f;
    
    // Calculate final damage with all modifiers
    UFUNCTION(BlueprintCallable, Category = "Combat")
    float CalculateFinalDamage(const AActor* Target) const;
};
```

## Item System

### Item Definition
```cpp
// Base item definition
UCLASS(Blueprintable)
class UItemDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Item name
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText ItemName;
    
    // Item description
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Description;
    
    // Item icon
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    UTexture2D* Icon;
    
    // Item weight (in pounds)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float Weight;
    
    // Item value (in gold pieces)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 Value;
    
    // Item type
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemType ItemType;
    
    // Stack size (1 for non-stackable items)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 MaxStackSize = 1;
    
    // Gameplay effects when equipped/used
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TArray<TSubclassOf<UGameplayEffect>> Effects;
};
```

### Inventory Item
```cpp
// Instance of an item in an inventory
USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    // Item definition
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UItemDefinition* ItemDef = nullptr;
    
    // Current stack size
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 StackSize = 1;
    
    // Is this item equipped?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    bool bIsEquipped = false;
    
    // Get the total weight of this item stack
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    float GetTotalWeight() const { return ItemDef ? ItemDef->Weight * StackSize : 0.0f; }
    
    // Can this item stack with another item?
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool CanStackWith(const FInventoryItem& Other) const;
};
```

## Save Game Data

### Character Save Data
```cpp
// Data needed to save/load a character
USTRUCT(BlueprintType)
struct FCharacterSaveData
{
    GENERATED_BODY()

    // Character name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FString CharacterName;
    
    // Character class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FName CharacterClass;
    
    // Character level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 Level = 1;
    
    // Experience points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 Experience = 0;
    
    // Attribute values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TMap<FName, float> Attributes;
    
    // Inventory items
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FInventoryItem> Inventory;
    
    // Equipped items
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TMap<EEquipmentSlot, FName> Equipment;
    
    // Current location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FTransform CurrentTransform;
    
    // Current level/map
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FName CurrentLevel;
};
```

## Serialization

### Save Game Wrapper
```cpp
// Wrapper for save game data
UCLASS()
class USaveGameWrapper : public USaveGame
{
    GENERATED_BODY()

public:
    // Save game version
    UPROPERTY(VisibleAnywhere, Category = "Save")
    int32 SaveVersion = 1;
    
    // Character data
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FCharacterSaveData CharacterData;
    
    // Game state data
    UPROPERTY(VisibleAnywhere, Category = "Save")
    TArray<uint8> GameStateData;
    
    // Save timestamp
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FDateTime SaveTime;
    
    // Save slot name
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FString SaveSlotName;
    
    // User index
    UPROPERTY(VisibleAnywhere, Category = "Save")
    int32 UserIndex = 0;
};
```

## Related Documents
- [Modular Design](modular_design.md)
- [Component System](component_system.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
