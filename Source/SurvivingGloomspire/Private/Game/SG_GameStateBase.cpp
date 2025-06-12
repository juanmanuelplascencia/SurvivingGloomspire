// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Game/SG_GameStateBase.h"
#include "Player/SG_PlayerState.h"
#include "SG_LogChannels.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagsManager.h"
#include "GameFramework/PlayerState.h"

ASG_GameStateBase::ASG_GameStateBase()
{
    // Replication settings
    bReplicates = true;
    SetNetUpdateFrequency(1.0f);
    SetMinNetUpdateFrequency(1.0f);
    
    // Initialize match state tags
    static const FName MatchStateTagNamespace = TEXT("MatchState");
    
    // Create native tags if they don't exist
    UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
    
    // Create or find the match state tags
    static FName WaitingTagName = FName(*(MatchStateTagNamespace.ToString() + TEXT(".WaitingToStart")));
    static FName InProgressTagName = FName(*(MatchStateTagNamespace.ToString() + TEXT(".InProgress")));
    
    // Create or find the match state tags
    MatchState_WaitingToStart = TagManager.AddNativeGameplayTag(
        WaitingTagName,
        TEXT("Match is waiting to start")
    );
    
    MatchState_InProgress = TagManager.AddNativeGameplayTag(
        InProgressTagName,
        TEXT("Match is in progress")
    );
    
    // Set default match state
    MatchState = MatchState_WaitingToStart;
}

void ASG_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ASG_GameStateBase, MatchState);
}

void ASG_GameStateBase::HandleBeginPlay()
{
    Super::HandleBeginPlay();
    
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameStateBase: Handling begin play"));
    
    // Set initial match state
    if (HasAuthority())
    {
        SetMatchState(MatchState_InProgress);
    }
}

void ASG_GameStateBase::AddPlayerState(APlayerState* PlayerState)
{
    Super::AddPlayerState(PlayerState);
    
    if (PlayerState)
    {
        UE_LOG(LogSG_Game, Log, TEXT("SG_GameStateBase: Player added: %s"), 
            *PlayerState->GetPlayerName());
            
        OnPlayerAdded(PlayerState);
    }
}

void ASG_GameStateBase::RemovePlayerState(APlayerState* PlayerState)
{
    if (PlayerState)
    {
        UE_LOG(LogSG_Game, Log, TEXT("SG_GameStateBase: Player removed: %s"), 
            *PlayerState->GetPlayerName());
            
        OnPlayerRemoved(PlayerState);
    }
    
    Super::RemovePlayerState(PlayerState);
}

bool ASG_GameStateBase::IsMatchInProgress() const
{
    return MatchState == MatchState_InProgress;
}

void ASG_GameStateBase::OnRep_MatchState()
{
    UE_LOG(LogSG_Game, Log, TEXT("SG_GameStateBase: Match state changed to: %s"), 
        *MatchState.ToString());
        
    OnMatchStateSet();
}

void ASG_GameStateBase::OnMatchStateSet()
{
    // Broadcast match state change
    // This can be used by UI or other systems to react to state changes
}

void ASG_GameStateBase::SetMatchState(FGameplayTag NewState)
{
    if (HasAuthority())
    {
        FGameplayTag OldState = MatchState;
        MatchState = NewState;
        
        // Notify clients of the state change
        OnRep_MatchState();
        
        // Notify the game mode if needed
        if (OldState != NewState)
        {
            OnMatchStateSet();
        }
        
        UE_LOG(LogSG_Game, Log, TEXT("Match state changed from %s to %s"), 
            *OldState.ToString(), 
            *NewState.ToString());
    }
}

void ASG_GameStateBase::OnPlayerAdded(APlayerState* PlayerState)
{
    // Handle player addition (e.g., update UI, stats, etc.)
    if (ASG_PlayerState* PS = Cast<ASG_PlayerState>(PlayerState))
    {
        // Initialize player state if needed
    }
}

void ASG_GameStateBase::OnPlayerRemoved(APlayerState* PlayerState)
{
    // Handle player removal (e.g., update UI, stats, etc.)
}
