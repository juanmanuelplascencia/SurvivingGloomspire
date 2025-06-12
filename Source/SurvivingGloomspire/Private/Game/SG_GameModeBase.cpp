// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Game/SG_GameModeBase.h"
#include "Game/SG_GameStateBase.h"
#include "Player/SG_PlayerController.h"
#include "Player/SG_PlayerState.h"
#include "SG_LogChannels.h"

ASG_GameModeBase::ASG_GameModeBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set default classes
    PlayerControllerClass = ASG_PlayerController::StaticClass();
    PlayerStateClass = ASG_PlayerState::StaticClass();
    GameStateClass = ASG_GameStateBase::StaticClass();
    // DefaultPawnClass will be set in the derived class or in the editor
    
    // Game settings
    bStartPlayersAsSpectators = false;
    bPauseable = true;
    bUseSeamlessTravel = true;
}

void ASG_GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameModeBase: Initializing game on map '%s' with options: %s"), 
        *MapName, 
        *Options);
    
    // Initialize game state
    InitializeGameState();
}

void ASG_GameModeBase::StartPlay()
{
    Super::StartPlay();
    
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameModeBase: Starting play"));
    
    // Initialize game systems
    InitializePlayerStates();
    InitializeAISystems();
    InitializeGameplaySystems();
    
    // Notify game started
    OnStart();
}

void ASG_GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameModeBase: Ending play (Reason: %d"), static_cast<int32>(EndPlayReason));
    
    // Notify game ending
    OnEnd();
    
    // Shutdown game systems
    ShutdownGameplaySystems();
    
    Super::EndPlay(EndPlayReason);
}

void ASG_GameModeBase::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
    
    if (NewPlayer && NewPlayer->GetPawn())
    {
        UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Restarted player %s"), 
            *GetNameSafe(NewPlayer->GetPawn()));
            
        OnPlayerRestarted(NewPlayer);
    }
}

void ASG_GameModeBase::OnStart()
{
    // Override in child classes
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameModeBase: Game started"));
}

void ASG_GameModeBase::OnEnd()
{
    // Override in child classes
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameModeBase: Game ended"));
}

void ASG_GameModeBase::OnPlayerRestarted(AController* NewPlayer)
{
    // Override in child classes
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Player restarted: %s"), 
        *GetNameSafe(NewPlayer));
}

void ASG_GameModeBase::InitializeGameState()
{
    if (GameState)
    {
        ASG_GameStateBase* GS = Cast<ASG_GameStateBase>(GameState);
        if (GS)
        {
            // Initialize game state here
            UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Initialized game state"));
        }
    }
}

void ASG_GameModeBase::InitializePlayerStates()
{
    // Initialize player states here
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Initialized player states"));
}

void ASG_GameModeBase::InitializeAISystems()
{
    // Initialize AI systems here
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Initialized AI systems"));
}

void ASG_GameModeBase::InitializeGameplaySystems()
{
    // Initialize any additional gameplay systems here
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Initialized gameplay systems"));
}

void ASG_GameModeBase::ShutdownGameplaySystems()
{
    // Shutdown any gameplay systems here
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_GameModeBase: Shutdown gameplay systems"));
}
