# Gameplay Ability System (GAS) Implementation

## Overview
This document details the implementation of Unreal Engine's Gameplay Ability System (GAS) for handling Pathfinder 1e's abilities, spells, and effects in a modular and data-driven way.

## Getting Started with GAS

### Enabling GAS
1. Add the following to your `ProjectName.Build.cs`:
```csharp
PublicDependencyModuleNames.AddRange(
    new string[] 
    { 
        "GameplayAbilities",
        "GameplayTags",
        "GameplayTasks"
    }
);
```

2. Enable the plugin in your project settings:
   - Edit > Plugins > Gameplay Abilities

### Core GAS Classes

#### Gameplay Ability System Component (GASC)
- Manages all abilities and attributes
- Handles networking and replication
- Processes gameplay effects

```cpp
// In your character class header
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
class UAbilitySystemComponent* AbilitySystemComponent;

// In your character class implementation
AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
AbilitySystemComponent->SetIsReplicated(true);
AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
```

#### Attribute Set
- Defines and manages character attributes
- Handles attribute modification

```cpp
UCLASS()
class MYGAME_API UMyAttributeSet : public UAttributeSet
{
    GENERATED_BODY()
    
public:
    // Core attributes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData Health;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FGameplayAttributeData MaxHealth;
    
    // ... other attributes
    
    // Attribute change callbacks
    void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
```

## Implementing Pathfinder Abilities

### Ability Base Class
```cpp
UCLASS()
class MYGAME_API UMyGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
    
public:
    UMyGameplayAbility();
    
    // Pathfinder-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    EAbilityType AbilityType;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FText AbilityName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FText AbilityDescription;
    
    // Override activation
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
    
    // Check if ability can be activated
    virtual bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags = nullptr,
        const FGameplayTagContainer* TargetTags = nullptr,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
```

### Spell Ability Example
```cpp
UCLASS()
class MYGAME_API UGA_CastSpell : public UMyGameplayAbility
{
    GENERATED_BODY()
    
public:
    UGA_CastSpell();
    
    // Spell properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell")
    TSubclassOf<UGameplayEffect> SpellEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell")
    float SpellRange;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell")
    float SpellRadius;
    
    // Implement activation
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
    
    // Handle spell targeting
    UFUNCTION(BlueprintCallable, Category = "Spell")
    void OnSpellTargetSelected(const FGameplayAbilityTargetDataHandle& TargetData);
    
    // Apply spell effects
    UFUNCTION(BlueprintCallable, Category = "Spell")
    void ApplySpellEffects(const FGameplayAbilityTargetDataHandle& TargetData);
};
```

## Gameplay Effects

### Effect Base Class
```cpp
UCLASS()
class MYGAME_API UMyGameplayEffect : public UGameplayEffect
{
    GENERATED_BODY()
    
public:
    UMyGameplayEffect();
    
    // Pathfinder-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    EEffectType EffectType;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    FText EffectName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    FText EffectDescription;
};
```

### Damage Effect Example
```cpp
UCLASS()
class MYGAME_API UGE_Damage : public UMyGameplayEffect
{
    GENERATED_BODY()
    
public:
    UGE_Damage();
    
    // Damage calculation function
    static float CalculateDamage(
        const FGameplayEffectSpec& Spec,
        const FAggregatorEvaluateParameters& EvaluationParams);
    
    // Critical hit calculation
    static float CalculateCriticalHit(
        const FGameplayEffectSpec& Spec,
        const FAggregatorEvaluateParameters& EvaluationParams,
        float BaseDamage);
};
```

## Ability System Setup

### Character Initialization
```cpp
void AMyCharacter::InitializeAttributes()
{
    if (AbilitySystemComponent && DefaultAttributeEffect)
    {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);
        
        FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
            DefaultAttributeEffect, 1, EffectContext);
            
        if (NewHandle.IsValid())
        {
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
        }
    }
}

void AMyCharacter::GiveAbilities()
{
    if (HasAuthority() && AbilitySystemComponent)
    {
        for (TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
        {
            if (Ability)
            {
                AbilitySystemComponent->GiveAbility(
                    FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
            }
        }
    }
}
```

## Networking and Replication

### Server-Client Communication
```cpp
// Server-side ability activation
void AMyCharacter::ServerActivateAbility_Implementation(FGameplayAbilitySpecHandle AbilityHandle, bool bInputPressed)
{
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->TryActivateAbility(AbilityHandle, bInputPressed);
    }
}

bool AMyCharacter::ServerActivateAbility_Validate(FGameplayAbilitySpecHandle AbilityHandle, bool bInputPressed)
{
    return true; // Add validation logic here
}
```

### Replication
```cpp
void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AMyCharacter, AbilitySystemComponent);
}
```

## UI Integration

### Ability Widget
```cpp
UCLASS()
class MYGAME_API UAbilityWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SetAbility(UGameplayAbility* NewAbility);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
    void OnAbilityCooldownChanged(float CooldownRemaining, float CooldownDuration);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
    void OnAbilityActivated();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
    void OnAbilityDeactivated();
    
private:
    UPROPERTY()
    UGameplayAbility* Ability;
};
```

## Debugging and Testing

### Console Commands
```
# List all abilities
AbilitySystem.DebugAbility

# Debug ability system component
AbilitySystem.DebugASC

# Show debug abilities
ShowDebug AbilitySystem

# Toggle ability debug
AbilitySystem.Debug.NextTarget
```

### Debug Drawing
```cpp
// Draw debug sphere for ability targeting
if (bShowDebug)
{
    DrawDebugSphere(
        GetWorld(),
        TargetLocation,
        AbilityRadius,
        16,
        FColor::Green,
        false,
        2.0f,
        0,
        2.0f
    );
}
```

## Best Practices

### Performance Optimization
1. Use ability tags for quick lookups
2. Minimize replication by using Gameplay Effects when possible
3. Use prediction for responsive gameplay

### Organization
1. Create a consistent folder structure for abilities and effects
2. Use naming conventions (e.g., `GA_` prefix for abilities, `GE_` for effects)
3. Document ability behavior and requirements

### Testing
1. Test abilities in standalone and networked environments
2. Verify replication and prediction behavior
3. Test edge cases and error conditions

## Related Documents
- [Paper2D Setup](paper2d_setup.md)
- [Data Management](data_management.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
