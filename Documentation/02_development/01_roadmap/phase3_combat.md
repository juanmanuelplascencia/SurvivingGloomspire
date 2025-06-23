# Phase 3: Combat Implementation

## Overview
This document details the implementation of the Pathfinder 1e combat system, including turn-based mechanics, action economy, and combat resolution.

## Goals
1. Implement turn-based combat system
2. Create combat actions and abilities
3. Develop AI for combat encounters
4. Implement combat UI and feedback

## Milestones

### 1. Turn System (Weeks 1-2)
- [ ] Implement initiative tracking
- [ ] Create turn order UI
- [ ] Develop action points system
- [ ] Implement delay/ready actions

### 2. Combat Actions (Weeks 3-5)
- [ ] Implement standard actions
- [ ] Create movement system
- [ ] Develop attack actions
- [ ] Implement special abilities

### 3. Combat Resolution (Weeks 6-8)
- [ ] Create damage system
- [ ] Implement conditions
- [ ] Develop combat log
- [ ] Create victory/defeat conditions

## Detailed Implementation

### Turn System

#### Initiative Tracking
```cpp
// Combat manager class
UCLASS()
class UCombatManager : public UObject
{
    GENERATED_BODY()
    
public:
    // Start combat with participants
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartCombat(const TArray<AActor*>& Participants);
    
    // Get current turn actor
    UFUNCTION(BlueprintCallable, Category = "Combat")
    AActor* GetCurrentActor() const;
    
    // Advance to next turn
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void NextTurn();
    
    // Get turn order
    UFUNCTION(BlueprintCallable, Category = "Combat")
    TArray<AActor*> GetTurnOrder() const;
    
private:
    // Current turn index
    int32 CurrentTurnIndex;
    
    // Turn order
    UPROPERTY()
    TArray<AActor*> TurnOrder;
    
    // Roll initiative for all participants
    void RollInitiative();
    
    // Sort participants by initiative
    void SortByInitiative();
};
```

### Action System

#### Action Types
```cpp
// Action types
UENUM(BlueprintType)
enum class ECombatActionType : uint8
{
    None        UMETA(DisplayName = "None"),
    Standard    UMETA(DisplayName = "Standard Action"),
    Move        UMETA(DisplayName = "Move Action"),
    Swift       UMETA(DisplayName = "Swift Action"),
    Immediate   UMETA(DisplayName = "Immediate Action"),
    Free        UMETA(DisplayName = "Free Action"),
    FullRound   UMETA(DisplayName = "Full-Round Action")
};

// Action cost
USTRUCT(BlueprintType)
struct FActionCost
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    ECombatActionType Type;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 Cost = 1;
};
```

#### Base Action Class
```cpp
// Base combat action
UCLASS(Abstract, Blueprintable)
class UCombatAction : public UObject
{
    GENERATED_BODY()
    
public:
    // Execute the action
    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    void Execute(AActor* Instigator, const FCombatActionContext& Context);
    
    // Can this action be executed?
    UFUNCTION(BlueprintNativeEvent, Category = "Combat")
    bool CanExecute(const AActor* Instigator, const FCombatActionContext& Context) const;
    
    // Get action cost
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
    virtual TArray<FActionCost> GetActionCost() const;
    
    // Get action name
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
    virtual FText GetActionName() const;
    
    // Get action description
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
    virtual FText GetActionDescription() const;
};
```

### Combat Resolution

#### Attack Resolution
```cpp
// Resolve an attack
bool UCombatSystem::ResolveAttack(
    AActor* Attacker, 
    AActor* Target, 
    const FAttackData& AttackData,
    FCombatResult& OutResult)
{
    if (!Attacker || !Target)
    {
        return false;
    }
    
    // Roll attack
    int32 AttackRoll = RollD20(Attacker);
    bool bIsCriticalThreat = (AttackRoll >= AttackData.CriticalThreatRange);
    
    // Check for critical hit confirmation
    if (bIsCriticalThreat)
    {
        int32 ConfirmRoll = RollD20(Attacker);
        OutResult.bCriticalHit = (ConfirmRoll + AttackData.AttackBonus) >= GetTargetAC(Target, AttackData.AttackType);
    }
    
    // Calculate total attack bonus
    int32 TotalAttackBonus = AttackData.AttackBonus;
    
    // Apply modifiers
    TotalAttackBonus += GetAttackBonusModifiers(Attacker, Target, AttackData);
    
    // Calculate total AC
    int32 TargetAC = GetTargetAC(Target, AttackData.AttackType);
    
    // Check for hit
    bool bHit = (AttackRoll == 20) || // Natural 20 always hits
               ((AttackRoll != 1) &&  // Natural 1 always misses
                ((AttackRoll + TotalAttackBonus) >= TargetAC));
    
    OutResult.bHit = bHit;
    OutResult.AttackRoll = AttackRoll;
    OutResult.TotalAttackBonus = TotalAttackBonus;
    OutResult.TargetAC = TargetAC;
    
    // Calculate damage if hit
    if (bHit)
    {
        OutResult.Damage = CalculateDamage(Attacker, Target, AttackData, OutResult.bCriticalHit);
    }
    
    return true;
}
```

### AI System

#### Combat AI
```cpp
// AI combat behavior
void UCombatAIController::UpdateCombatBehavior(float DeltaTime)
{
    if (!CombatManager || !ControlledPawn)
    {
        return;
    }
    
    // Get current state
    AActor* CurrentTarget = GetCurrentTarget();
    
    // If no target, find one
    if (!CurrentTarget)
    {
        CurrentTarget = FindNewTarget();
        if (!CurrentTarget)
        {
            // No valid targets
            return;
        }
    }
    
    // Check if target is in range
    float DistanceToTarget = FVector::Distance(
        ControlledPawn->GetActorLocation(),
        CurrentTarget->GetActorLocation());
    
    // Select best action based on situation
    UCombatAction* BestAction = nullptr;
    float BestScore = -1.0f;
    
    for (UCombatAction* Action : AvailableActions)
    {
        if (Action && Action->CanExecute(ControlledPawn, FCombatActionContext(CurrentTarget)))
        {
            float Score = ScoreAction(Action, CurrentTarget);
            if (Score > BestScore)
            {
                BestScore = Score;
                BestAction = Action;
            }
        }
    }
    
    // Execute best action
    if (BestAction)
    {
        FCombatActionContext Context(CurrentTarget);
        BestAction->Execute(ControlledPawn, Context);
    }
}
```

## UI Components

### Combat HUD
```cpp
// Combat HUD widget
UCLASS()
class UCombatHUD : public UUserWidget
{
    GENERATED_BODY()
    
public:
    // Update turn order display
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateTurnOrder(const TArray<AActor*>& TurnOrder, int32 CurrentTurnIndex);
    
    // Show action menu
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowActionMenu(AActor* TargetActor, const TArray<UCombatAction*>& AvailableActions);
    
    // Update character stats
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateCharacterStats(AActor* Character);
    
    // Show combat log message
    UFUNCTION(BlueprintCallable, Category = "UI")
    void AddCombatLogMessage(const FText& Message);
    
    // Show damage numbers
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void ShowDamageNumber(AActor* Target, int32 Amount, bool bIsCritical);
    
    // Show floating text
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void ShowFloatingText(AActor* Target, const FText& Text, FLinearColor Color);
};
```

## Testing Plan

### Unit Tests
- Initiative calculation
- Attack resolution
- Damage calculation
- Condition application

### Integration Tests
- Full combat round
- Multiple combatants
- Complex scenarios
- Edge cases

### Performance Testing
- Large scale battles
- Long combat sessions
- Memory usage
- Frame rate

## Risk Assessment

### Technical Risks
1. **Performance with Many Combatants**
   - Mitigation: Implement level of detail
   - Optimize AI calculations

2. **Complex State Management**
   - Mitigation: Use state pattern
   - Implement comprehensive logging

3. **Network Synchronization**
   - Mitigation: Use UE's networking
   - Implement client-side prediction

### Schedule Risks
1. **AI Complexity**
   - Mitigation: Start with simple behaviors
   - Iteratively improve

2. **UI Polish Time**
   - Mitigation: Use placeholders initially
   - Focus on functionality first

## Success Criteria
1. Functional turn-based combat
2. Working AI opponents
3. Complete combat UI
4. Comprehensive test coverage
5. Performance targets met

## Next Steps
1. Begin Phase 4: Content Creation
2. Implement additional combat features
3. Polish combat feedback

## Related Documents
- [Phase 2: Core Systems](phase2_core_systems.md)
- [Phase 4: Content Creation](phase4_content_creation.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
