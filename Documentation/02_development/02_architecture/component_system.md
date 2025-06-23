# Component-Based Architecture

## Overview
This document details the Entity-Component-System (ECS) architecture used in the Pathfinder 1e implementation, providing a flexible and performant foundation for character abilities, items, and game mechanics.

## Core Concepts

### Entities
- Simple containers for components
- Represent game objects (characters, items, effects)
- Identified by unique IDs

### Components
- Pure data containers
- No behavior or logic
- Examples: Position, Health, Inventory

### Systems
- Contain all game logic
- Operate on components
- Run in specific order

## Implementation in Unreal Engine

### Base Component Class
```cpp
// Base component class
UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UGameComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Called when component is initialized
    virtual void InitializeComponent() override;

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Get the owning character
    UFUNCTION(BlueprintCallable, Category = "Component")
    virtual class ACharacterBase* GetOwnerCharacter() const;

    // Component initialization data
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
    FName ComponentID;

protected:
    // Is this component currently active?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
    bool bIsActive = true;
};
```

### Example: Health Component
```cpp
// Health component
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UHealthComponent : public UGameComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    // Initialize with default values
    virtual void InitializeComponent() override;

    // Apply damage to the character
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

    // Get current health percentage
    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

    // Health changed event
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHealthChanged OnHealthChanged;

    // Death event
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeath OnDeath;

protected:
    // Current health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float CurrentHealth;

    // Maximum health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    // Is the character dead?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead;
};
```

## Component Registration

### Component Manager
```cpp
// Manages all component types
UCLASS()
class COMPONENTLIBRARY_API UComponentManager : public UObject
{
    GENERATED_BODY()

public:
    // Get the global component manager
    static UComponentManager* Get();

    // Register a component type
    template<typename T>
    void RegisterComponentType()
    {
        const FName TypeName = T::StaticClass()->GetFName();
        ComponentTypes.Add(TypeName, T::StaticClass());
    }

    // Create a component by name
    UGameComponent* CreateComponent(FName ComponentType, AActor* Owner);

private:
    // Map of component type names to their classes
    TMap<FName, TSubclassOf<UGameComponent>> ComponentTypes;
};
```

### Component Initialization
```cpp
// Component initialization function
void InitializeComponents(AActor* Owner)
{
    // Get all component descriptors
    TArray<FComponentDescriptor> ComponentsToCreate = GetComponentDescriptors();
    
    // Create each component
    for (const FComponentDescriptor& Descriptor : ComponentsToCreate)
    {
        if (UGameComponent* Component = UComponentManager::Get()->CreateComponent(Descriptor.Type, Owner))
        {
            // Configure component from descriptor
            Component->ComponentID = Descriptor.ID;
            
            // Add to owner
            Component->RegisterComponent();
            Owner->AddInstanceComponent(Component);
            Component->AttachToComponent(Owner->GetRootComponent(), 
                FAttachmentTransformRules::KeepRelativeTransform);
                
            // Initialize if we're already in play
            if (Owner->HasActorBegunPlay())
            {
                Component->InitializeComponent();
                Component->BeginPlay();
            }
        }
    }
}
```

## Component Communication

### Event System
```cpp
// Base event class
USTRUCT(BlueprintType)
struct FGameEvent
{
    GENERATED_BODY()

    // Event type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    FName EventType;

    
    // Source component
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    UGameComponent* Source = nullptr;
    
    // Target component (optional)
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    UGameComponent* Target = nullptr;
    
    // Event payload
    UPROPERTY(BlueprintReadOnly, Category = "Event")
    TMap<FString, FString> Payload;
};

// Event dispatcher delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEvent, const FGameEvent&, Event);

// Component with event support
UCLASS()
class UEventComponent : public UGameComponent
{
    GENERATED_BODY()

public:
    // Broadcast an event
    UFUNCTION(BlueprintCallable, Category = "Event")
    void BroadcastEvent(const FGameEvent& Event);
    
    // Subscribe to an event type
    UFUNCTION(BlueprintCallable, Category = "Event")
    void SubscribeToEvent(FName EventType, UGameComponent* Subscriber);
    
    // Event received delegate
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnGameEvent OnEventReceived;
    
private:
    // Map of event types to subscribers
    TMap<FName, TArray<UGameComponent*>> EventSubscriptions;
};
```

## Component Lifecycle

### Initialization Flow
1. **Registration**
   - Component classes register with the ComponentManager
   - Typically done during game module startup

2. **Creation**
   - Components are created based on data definitions
   - Added to their parent actor
   - Registered with the game world

3. **Initialization**
   - InitializeComponent() is called
   - Components set up their initial state
   - Register for events

4. **Runtime**
   - TickComponent() called each frame
   - Process incoming events
   - Update component state

5. **Shutdown**
   - Components unregister from events
   - Release resources
   - Prepare for destruction

## Performance Considerations

### Component Pools
- Reuse components instead of creating/destroying
- Especially important for frequently used components
- Reduces memory allocation overhead

### Update Optimization
- Only update components that need it
- Use update priorities
- Batch similar operations

### Memory Layout
- Optimize for cache efficiency
- Group frequently accessed data together
- Consider data-oriented design for performance-critical systems

## Best Practices

### Component Design
1. Keep components focused on a single responsibility
2. Minimize dependencies between components
3. Use events for cross-component communication
4. Make components as reusable as possible
5. Document component interfaces clearly

### Performance
1. Profile component performance regularly
2. Use object pooling where appropriate
3. Be mindful of memory usage
4. Optimize hot paths

## Related Documents
- [Modular Design](modular_design.md)
- [Data Structures](data_structures.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
