// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameplayTagContainer.h"
#include "SG_GameStateBase.generated.h"

/**
 * Base game state class for Surviving Gloomspire
 * Tracks the state of the game (e.g., match state, player states, etc.)
 */
UCLASS()
class SURVIVINGGLOOMSPIRE_API ASG_GameStateBase : public AGameStateBase
{
    GENERATED_BODY()
    
public:
    ASG_GameStateBase();
    
    //~ Begin AGameStateBase Interface
    virtual void HandleBeginPlay() override;
    virtual void AddPlayerState(APlayerState* PlayerState) override;
    virtual void RemovePlayerState(APlayerState* PlayerState) override;
    //~ End AGameStateBase Interface
    
    /** Get the current match state */
    UFUNCTION(BlueprintCallable, Category = "GameState")
    FGameplayTag GetMatchState() const { return MatchState; }
    
    /** Check if the match is in progress */
    UFUNCTION(BlueprintCallable, Category = "GameState")
    bool IsMatchInProgress() const;
    
protected:
    /** Current match state */
    UPROPERTY(ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, Category = "GameState")
    FGameplayTag MatchState;
    
    /** Replication */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    /** Match state tag for waiting to start */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState")
    FGameplayTag MatchState_WaitingToStart;
    
    /** Match state tag for in progress */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState")
    FGameplayTag MatchState_InProgress;
    
    /** Called when the match state is replicated */
    UFUNCTION()
    virtual void OnRep_MatchState();
    
    /** Called when the match state changes */
    virtual void OnMatchStateSet();
    
    /** Set the current match state */
    virtual void SetMatchState(FGameplayTag NewState);
    
    /** Called when a player is added to the game */
    virtual void OnPlayerAdded(APlayerState* PlayerState);
    
    /** Called when a player is removed from the game */
    virtual void OnPlayerRemoved(APlayerState* PlayerState);
};
