// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_GameModeBase.generated.h"

/**
 * Base game mode class for Surviving Gloomspire
 * Extend this class for specific game modes
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class SURVIVINGGLOOMSPIRE_API ASG_GameModeBase : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    /** Constructor */
    ASG_GameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    //~ Begin AGameModeBase Interface
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void RestartPlayer(AController* NewPlayer) override;
    //~ End AGameModeBase Interface
    
protected:
    /** Called when the game starts */
    virtual void OnStart();
    
    /** Called when the game ends */
    virtual void OnEnd();
    
    /** Called when a player is restarted */
    virtual void OnPlayerRestarted(AController* NewPlayer);
    
    /** Initialize game state */
    virtual void InitializeGameState();
    
    /** Initialize player states */
    virtual void InitializePlayerStates();
    
    /** Initialize AI systems */
    virtual void InitializeAISystems();
    
    /** Initialize game-specific systems */
    virtual void InitializeGameplaySystems();
    
    /** Shutdown game-specific systems */
    virtual void ShutdownGameplaySystems();
};
