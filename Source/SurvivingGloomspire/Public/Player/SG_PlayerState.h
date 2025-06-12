// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SG_PlayerState.generated.h"

/**
 * Player state for Surviving Gloomspire
 * Tracks player-specific data that persists between levels and matches
 */
UCLASS()
class SURVIVINGGLOOMSPIRE_API ASG_PlayerState : public APlayerState
{
    GENERATED_BODY()
    
public:
    ASG_PlayerState();
    
    //~ Begin APlayerState Interface
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void CopyProperties(APlayerState* PlayerState) override;
    virtual void OverrideWith(APlayerState* PlayerState) override;
    //~ End APlayerState Interface
    
    /** Get the player's team ID */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    int32 GetTeamId() const { return TeamId; }
    
    /** Set the player's team ID */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    void SetTeamId(int32 NewTeamId);
    
    /** Get the player's score */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    int32 GetPlayerScore() const { return GetScore(); }
    
    /** Add to the player's score */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    void AddScore(int32 ScoreToAdd);
    
    /** Get the player's kills */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    int32 GetKills() const { return Kills; }
    
    /** Add a kill to the player's count */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    void AddKill();
    
    /** Get the player's deaths */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    int32 GetDeaths() const { return Deaths; }
    
    /** Add a death to the player's count */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    void AddDeath();
    
protected:
    /** The team this player is on */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
    int32 TeamId;
    
    /** Number of kills this player has */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
    int32 Kills;
    
    /** Number of deaths this player has */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
    int32 Deaths;
    
    /** Called when the team ID is replicated */
    UFUNCTION()
    virtual void OnRep_TeamId();
    
    /** Called when the kill count is replicated */
    UFUNCTION()
    virtual void OnRep_Kills();
    
    /** Called when the death count is replicated */
    UFUNCTION()
    virtual void OnRep_Deaths();
};
