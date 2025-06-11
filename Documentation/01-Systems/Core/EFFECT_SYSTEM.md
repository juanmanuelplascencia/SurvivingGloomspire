# Effect System

## Overview
The Effect System (`IEffectSystem`) is a comprehensive framework for managing all in-game effects, including status effects, buffs, debuffs, and environmental modifiers. It provides a robust, performant, and flexible way to handle complex effect interactions while maintaining game balance and performance.

## Table of Contents
1. [Core Concepts](#core-concepts)
2. [Architecture](#architecture)
   - [Effect Structure](#effect-structure)
   - [System Components](#system-components)
3. [Usage Guide](#usage-guide)
   - [Effect Types](#effect-types)
   - [Common Patterns](#common-patterns)
   - [Best Practices](#best-practices)
4. [Implementation Details](#implementation-details)
   - [Performance Considerations](#performance-considerations)
   - [Error Handling](#error-handling)
   - [Security Features](#security-features)
5. [API Reference](#api-reference)
6. [Troubleshooting](#troubleshooting)
7. [Future Improvements](#future-improvements)

## Core Concepts

### What is an Effect?
An effect is a temporary or permanent modifier that can be applied to game objects, characters, or the environment. Effects can modify attributes, apply visual/audio feedback, trigger abilities, or interact with other game systems.

### Key Concepts
- **Effect Stacking**: Multiple instances of the same effect can be combined
- **Duration Management**: Effects can be temporary or permanent
- **Effect Chaining**: Effects can trigger other effects
- **Effect Categories**: Different types of effects for different purposes
- **Effect Sources**: Track what caused an effect to be applied

## Architecture

### Effect Structure

Effects are defined using the following structure:

```typescript
interface IGameEffect {
    // Core Properties
    id: string;                    // Unique identifier
    name: string;                  // Display name
    description: string;           // Effect description
    type: EffectType;              // Type of effect (buff, debuff, etc.)
    category: EffectCategory;      // Effect category
    
    // Timing and Stacking
    duration: number;              // Effect duration in seconds
    maxStacks: number;             // Maximum number of stacks
    currentStacks: number;         // Current number of stacks
    tickInterval: number;          // Time between effect ticks
    
    // State Flags
    isDebuff: boolean;             // Whether this is a negative effect
    isHidden: boolean;             // If true, effect is not shown in UI
    isUnique: boolean;             // If true, only one instance can be active
    isPersistent: boolean;         // If true, effect persists through death
    isDispellable: boolean;        // If true, can be removed by dispel effects
    isPurgeable: boolean;          // If true, can be removed by purge effects
    isRefreshable: boolean;        // If true, duration can be refreshed
    isStackable: boolean;          // If true, can have multiple stacks
    isPermanent: boolean;          // If true, effect never expires
    
    // Visual/Audio Feedback
    icon: string;                  // UI icon
    particleEffect: string;        // Visual effect
    soundEffect: string;           // Audio effect
    
    // Metadata
    metadata: EffectMetadata;      // Additional effect data
    validation: EffectValidation;  // Validation rules
    security: EffectSecurity;      // Security settings
    
    // Callbacks
    onApply: EffectCallback;       // Called when effect is applied
    onTick: EffectCallback;        // Called each tick
    onRemove: EffectCallback;      // Called when effect is removed
    onStack: EffectCallback;       // Called when effect is stacked
    onRefresh: EffectCallback;     // Called when effect is refreshed
    onDispel: EffectCallback;      // Called when effect is dispelled
    onPurge: EffectCallback;       // Called when effect is purged
    onExpire: EffectCallback;      // Called when effect expires
    
    // Versioning
    version: string;               // Effect version
    created: Date;                 // Creation timestamp
    modified: Date;                // Last modified timestamp
    source: string;                // Source of the effect
    
    // Debug Information
    debug: {
        isDebug: boolean;         // Debug mode flag
        sourceFile: string;        // Source file path
        sourceLine: number;        // Source line number
        stackTrace: string[];      // Stack trace when created
    };
}
```

### System Components

The Effect System consists of several key components:

1. **Effect Manager**
   - Central controller for all effect operations
   - Handles effect application, updates, and removal
   - Manages effect stacking and combination rules

2. **Effect Handlers**
   - Type-specific behavior implementations
   - Handles effect application and cleanup
   - Manages effect-specific state

3. **Effect Context**
   - Tracks effect source and target
   - Contains metadata about effect application
   - Used for effect validation and modification

## Usage Guide

### Effect Types

| Type | Description | Example |
|------|-------------|---------|
| **Instant** | Applied immediately once | Damage, Healing |
| **Duration** | Active for a set time | Buffs, Debuffs |
| **Periodic** | Triggers at intervals | Damage over Time, Regeneration |
| **Permanent** | Lasts until removed | Equipment Bonuses |
| **Toggle** | Can be toggled on/off | Auras, Toggle Abilities |

### Common Patterns

#### 1. Applying an Effect
```cpp
// Create effect spec
FGameplayEffectSpec Spec(EffectDef, Context);
Spec.SetLevel(CharacterLevel);

// Apply to target
FActiveGameplayEffectHandle Handle = AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);
```

#### 2. Stacking Effects
```cpp
// Check if effect can stack
if (ExistingEffect && ExistingEffect->CanStackWith(NewEffect))
{
    ExistingEffect->AddStack(NewEffect);
}
```

#### 3. Periodic Effects
```cpp
// In effect definition
Period = 1.0f;  // 1 second between ticks

// In tick function
void UMyEffect::ExecutePeriodicEffect()
{
    // Apply effect logic
    ApplyDamage();
}
```

## Implementation Details

### Effect Application Flow
1. Effect validation
2. Security checks
3. Target validation
4. Stack calculation
5. Effect combination
6. Effect modification
7. Effect application
8. OnApply callback
9. Effect tracking
10. Duration management
11. Tick processing
12. Effect refresh
13. Effect removal
14. OnRemove callback
15. Cleanup

### Performance Considerations

#### Optimization Techniques
- **Object Pooling**: Reuse effect instances
- **Batch Processing**: Process multiple effects in a single update
- **Lazy Evaluation**: Only update effects when needed
- **Effect Culling**: Skip updates for inactive effects
- **Update Batching**: Group effect updates by type

#### Memory Management
- Use memory pools for effect instances
- Implement reference counting
- Optimize effect data structures
- Use bit flags for boolean states
- Implement custom allocators for effect data

### Error Handling

#### Common Issues
1. Effect application failures
2. Stack overflow in effect chains
3. Memory leaks from effect references
4. Infinite effect loops
5. Race conditions in effect updates

#### Recovery Strategies
- Automatic effect cleanup
- Reference validation
- Cycle detection
- Safe effect removal
- State restoration on failure

### Security Features

#### Protection Mechanisms
- Input validation
- Effect permission checks
- Source verification
- Effect limits
- Rate limiting

#### Anti-Cheat
- Server-side validation
- Effect verification
- Cheat detection
- Logging and reporting
- Client prediction validation

## API Reference

### Core Interfaces

```cpp
// Main effect system interface
class IEffectSystem
{
public:
    virtual ~IEffectSystem() = default;
    
    // Effect management
    virtual FEffectHandle ApplyEffect(
        AActor* Target, 
        const FGameplayEffectSpec& Spec,
        const FGameplayEffectContext& Context
    ) = 0;
    
    virtual bool RemoveEffect(const FEffectHandle& Handle) = 0;
    
    // Effect querying
    virtual bool HasMatchingEffect(
        const FGameplayEffectQuery& Query
    ) const = 0;
    
    // System management
    virtual void Update(float DeltaTime) = 0;
};
```

### Common Tasks

#### Creating an Effect
```cpp
// Create effect definition
UGameplayEffect* Effect = NewObject<UGameplayEffect>();
Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
Effect->DurationMagnitude = 10.0f;

// Add modifiers
FGameplayModifierInfo Modifier;
Modifier.Attribute = UMyAttributeSet::GetHealthAttribute();
Modifier.ModifierOp = EGameplayModOp::Additive;
Modifier.ModifierMagnitude = FScalableFloat(10.0f);
Effect->Modifiers.Add(Modifier);
```

#### Applying an Effect
```cpp
// Create effect context
FGameplayEffectContextHandle Context = MakeShared<FGameplayEffectContext>();
Context.AddInstigator(Instigator, nullptr);

// Create effect spec
FGameplayEffectSpecHandle Spec = 
    MakeOutgoingSpec(EffectClass, 1.0f, Context);

// Apply to target
ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
```

## Troubleshooting

### Common Issues

#### Effect Not Applying
1. Check effect prerequisites
2. Verify target is valid
3. Check effect stacking rules
4. Verify effect permissions
5. Check for immunity

#### Performance Problems
1. Profile effect updates
2. Check for effect leaks
3. Verify effect pooling
4. Monitor memory usage
5. Check for redundant updates

## Future Improvements

### Short-term
1. Optimize effect updates
2. Improve effect debugging tools
3. Add more effect visualization

### Long-term
1. Advanced effect prediction
2. Better effect authoring tools
3. Enhanced effect networking
4. Machine learning for effect balancing
5. Procedural effect generation
```typescript
interface IGameEffect {
    // Core Properties
    id: string;
    name: string;
    description: string;
    type: EffectType;
    category: EffectCategory;
    subcategory: EffectSubcategory;
    
    // Timing and Stacking
    duration: number;
    remainingTime: number;
    maxStacks: number;
    currentStacks: number;
    tickInterval: number;
    
    // Flags
    isDebuff: boolean;
    isHidden: boolean;
    isUnique: boolean;
    isPersistent: boolean;
    isDispellable: boolean;
    isPurgeable: boolean;
    isRefreshable: boolean;
    isStackable: boolean;
    isPermanent: boolean;
    
    // Visuals and Feedback
    icon: string;
    particleEffect: string;
    soundEffect: string;
    visualEffect: string;
    
    // Metadata
    metadata: EffectMetadata;
    validation: EffectValidation;
    security: EffectSecurity;
    recovery: EffectRecovery;
    performance: EffectPerformance;
    
    // Callbacks
    onApply: EffectCallback;
    onTick: EffectCallback;
    onRemove: EffectCallback;
    onStack: EffectCallback;
    onRefresh: EffectCallback;
    onDispel: EffectCallback;
    onPurge: EffectCallback;
    onExpire: EffectCallback;
    
    // Versioning and Tracking
    version: string;
    created: Date;
    modified: Date;
    source: string;
    
    // Debug Information
    debug: {
        isDebug: boolean;
        sourceFile: string;
        sourceLine: number;
        stackTrace: string[];
    };
}
```

### Effect Application Flow
1. Effect validation
2. Security checks
3. Target validation
4. Stack calculation
5. Effect combination
6. Effect modification
7. Effect application
8. OnApply callback
9. Effect tracking
10. Duration management
11. Tick processing
12. Effect refresh
13. Effect removal
14. OnRemove callback
15. Cleanup
16. Recovery procedures
17. Logging
18. Monitoring
19. Debugging
20. Performance profiling

## Performance Considerations
- Effect pooling
- Batch processing
- Lazy evaluation
- Memory management
- Thread safety
- Effect culling
- Update frequency optimization
- Effect batching
- Performance profiling
- Memory optimization
- Network optimization
- Rendering optimization
- Update optimization
- Memory pooling
- Garbage collection
- Effect prioritization
- Effect culling
- Effect batching
- Effect optimization

## Error Handling
- Effect validation errors
- Application errors
- Stack overflow protection
- Duration overflow protection
- Recovery procedures
- Error reporting
- Effect cleanup
- State restoration
- Error logging
- Debug visualization
- Network errors
- Security errors
- Performance errors
- Recovery errors
- Validation errors
- Serialization errors
- Deserialization errors
- Network prediction errors
- Replay errors
- Analysis errors

## Security Features
- Effect validation
- Security checks
- Access control
- Input validation
- Output sanitization
- Error handling
- Logging
- Monitoring
- Recovery
- Security compliance
- Network security
- Anti-cheat
- Validation
- Authorization
- Authentication
- Encryption
- Signing
- Verification
- Auditing
- Reporting

## Best Practices
1. Use effect pooling
2. Implement proper cleanup
3. Handle effect stacking
4. Monitor effect performance
5. Use effect categories
6. Implement effect validation
7. Handle effect removal
8. Use effect callbacks
9. Monitor effect memory
10. Implement effect recovery
11. Use effect security
12. Monitor effect security
13. Implement effect debugging
14. Use effect optimization
15. Monitor effect optimization
16. Implement effect monitoring
17. Use effect analysis
18. Monitor effect analysis
19. Implement effect reporting
20. Use effect documentation

## Platform-Specific Considerations
- Platform-specific effect implementations
- Performance optimization
- Security considerations
- Recovery procedures
- Monitoring and debugging
- Platform-specific optimizations
- Platform-specific features
- Platform-specific limitations
- Platform-specific testing
- Platform-specific validation

## Future Improvements
1. Enhanced effect optimization
2. Improved effect validation
3. Better effect recovery
4. Enhanced effect security
5. Improved effect performance
6. Better platform support
7. Enhanced effect compatibility
8. Improved effect reliability
9. Better effect optimization
10. Enhanced effect user experience
11. Improved effect networking
12. Better effect prediction
13. Enhanced effect replay
14. Improved effect history
15. Better effect analysis
16. Enhanced effect reporting
17. Improved effect documentation
18. Better effect testing
19. Enhanced effect debugging
20. Improved effect monitoring

## Troubleshooting
1. Effect application failures
2. Performance issues
3. Platform-specific issues
4. Security validation issues
5. Effect recovery issues
6. Compatibility issues
7. Network issues
8. Prediction issues
9. Replay issues
10. Analysis issues
11. Reporting issues
12. Documentation issues
13. Testing issues
14. Debugging issues
15. Monitoring issues

## Performance Metrics
- Effect application time
- Effect update time
- Effect removal time
- Memory usage
- CPU usage
- Network bandwidth
- Network latency
- Prediction accuracy
- Replay accuracy
- Analysis time

## Security Validation
- Effect integrity
- Security validation
- Access control
- Input validation
- Output validation
- Error handling
- Logging
- Monitoring
- Recovery
- Security compliance
- Network security
- Anti-cheat
- Validation
- Authorization
- Authentication
- Encryption
- Signing
- Verification
- Auditing
- Reporting

## User Experience
- Clear effect descriptions
- Visual feedback
- Audio feedback
- Haptic feedback
- Effect duration indicators
- Stack indicators
- Source indicators
- Removal indicators
- Error messages
- Debug information
- Performance feedback
- Security feedback
- Recovery options
- Validation feedback
- Monitoring feedback
- Optimization suggestions
- Documentation
- Support resources
- Tutorials
- Tooltips

## Interface

```cpp
/**
 * Effect application result
 */
enum class EEffectApplicationResult
{
    Applied,        // Effect was applied successfully
    Immune,         // Target is immune to this effect
    Resisted,       // Target resisted the effect
    NoEffect,       // Effect had no effect
    Failed          // Application failed
};

/**
 * Base effect interface
 */
class IEffect
{
public:
    virtual ~IEffect() = default;
    
    // Effect lifecycle
    virtual EEffectApplicationResult Apply(FEffectContext& Context) = 0;
    virtual void Update(float DeltaTime) = 0;
    virtual void Remove() = 0;
    
    // Effect properties
    virtual FName GetEffectId() const = 0;
    virtual float GetDuration() const = 0;
    virtual int32 GetStacks() const = 0;
    virtual bool IsExpired() const = 0;
    
    // Stacking
    virtual bool CanStackWith(const TSharedPtr<IEffect>& Other) const = 0;
    virtual void CombineWith(TSharedPtr<IEffect> Other) = 0;
};

/**
 * Effect system interface
 */
class IEffectSystem
{
public:
    virtual ~IEffectSystem() = 0;
    
    // Effect management
    virtual TSharedPtr<IEffect> ApplyEffect(TSubclassOf<UEffect> EffectClass, AActor* Target, 
                                          AActor* Instigator = nullptr) = 0;
    virtual bool RemoveEffect(TSharedPtr<IEffect> Effect) = 0;
    virtual void RemoveAllEffects(AActor* Target) = 0;
    
    // Effect querying
    virtual TArray<TSharedPtr<IEffect>> GetEffects(AActor* Target) const = 0;
    virtual bool HasEffect(TSubclassOf<UEffect> EffectClass, AActor* Target) const = 0;
    virtual int32 GetEffectCount(TSubclassOf<UEffect> EffectClass, AActor* Target) const = 0;
    
    // Effect events
    DECLARE_EVENT_TwoParams(IEffectSystem, FEffectEvent, TSharedPtr<IEffect>, AActor*)
    virtual FEffectEvent& OnEffectApplied() = 0;
    virtual FEffectEvent& OnEffectRemoved() = 0;
    
    // System management
    virtual void Update(float DeltaTime) = 0;
    virtual void Clear() = 0;
};
```

## Implementation Details

### Core Components

1. **Effect Manager**
   - Tracks all active effects
   - Handles effect application and removal
   - Manages effect stacking and combination

2. **Effect Handlers**
   - Type-specific effect behavior
   - Custom logic for different effect types
   - Handles effect expiration and cleanup

3. **Effect Context**
   - Contains effect application data
   - Tracks source, target, and parameters
   - Handles effect chaining

### Performance Considerations
- Object pooling for effect instances
- Efficient effect lookup and iteration
- Batch updates for performance-critical paths
- Minimal memory allocation during updates

## Usage Examples

### Creating an Effect
```cpp
class FDamageEffect : public IEffect
{
public:
    FDamageEffect(float InDamage) : Damage(InDamage) {}
    
    virtual EEffectApplicationResult Apply(FEffectContext& Context) override
    {
        if (auto* TargetCharacter = Cast<IGameCharacter>(Context.Target))
        {
            TargetCharacter->TakeDamage(Damage, Context.Instigator);
            return EEffectApplicationResult::Applied;
        }
        return EEffectApplicationResult::Failed;
    }
    
    // ... other IEffect overrides
    
private:
    float Damage;
};
```

### Applying Effects
```cpp
// Apply a damage effect
auto Effect = MakeShared<FDamageEffect>(25.0f);
EffectSystem->ApplyEffect(Effect, Enemy, Player);

// Apply a buff with duration
auto Buff = MakeShared<FStatBuffEffect>(EStatType::Strength, 2, 30.0f);
EffectSystem->ApplyEffect(Buff, Player, nullptr);
```

### Handling Effect Events
```cpp
// Subscribe to effect events
EffectSystem->OnEffectApplied().AddLambda([](TSharedPtr<IEffect> Effect, AActor* Target) {
    if (auto* Character = Cast<IGameCharacter>(Target))
    {
        Character->OnEffectApplied(Effect);
    }
});
```

## Best Practices

1. **Effect Design**
   - Keep effects focused and single-purpose
   - Use composition for complex effects
   - Implement proper cleanup in destructors

2. **Performance**
   - Minimize allocations in effect updates
   - Use object pooling for frequently created effects
   - Batch effect updates when possible

3. **Debugging**
   - Add debug visualization for active effects
   - Log important effect events
   - Include effect names in debug output

## Configuration

Effects can be defined in data assets:

```json
{
    "Effects": [
        {
            "Id": "Poison",
            "Type": "Periodic",
            "Duration": 30.0,
            "Interval": 1.0,
            "Stacking": "Refresh",
            "Modifiers": [
                {
                    "Type": "Health",
                    "Operation": "Subtract",
                    "Value": 5.0
                }
            ]
        }
    ]
}
```

## Integration

The Effect System integrates with other systems:
- **Combat System**: Applies damage and status effects
- **Ability System**: Triggers effects from abilities
- **UI System**: Displays active effects on characters
- **Save System**: Saves and loads effect states
