// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Player/SG_PlayerState.h"
#include "SG_LogChannels.h"
#include "Net/UnrealNetwork.h"

ASG_PlayerState::ASG_PlayerState()
{
    // Set default values
    TeamId = 0;
    Kills = 0;
    Deaths = 0;
    
    // Replication settings
    bReplicates = true;
    SetNetUpdateFrequency(10.0f);
    SetReplicates(true);
    
    // Initialize score
    SetScore(0.0f);
}

void ASG_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(ASG_PlayerState, TeamId);
    DOREPLIFETIME(ASG_PlayerState, Kills);
    DOREPLIFETIME(ASG_PlayerState, Deaths);
}

void ASG_PlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);
    
    if (ASG_PlayerState* PS = Cast<ASG_PlayerState>(PlayerState))
    {
        PS->TeamId = TeamId;
        PS->Kills = Kills;
        PS->Deaths = Deaths;
    }
}

void ASG_PlayerState::OverrideWith(APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);
    
    if (ASG_PlayerState* PS = Cast<ASG_PlayerState>(PlayerState))
    {
        TeamId = PS->TeamId;
        Kills = PS->Kills;
        Deaths = PS->Deaths;
    }
}

void ASG_PlayerState::SetTeamId(int32 NewTeamId)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        int32 OldTeamId = TeamId;
        TeamId = NewTeamId;
        
        // Notify clients of the change
        OnRep_TeamId();
        
        UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerState: %s team changed from %d to %d"), 
            *GetPlayerName(), 
            OldTeamId, 
            TeamId);
    }
}

void ASG_PlayerState::AddScore(int32 ScoreToAdd)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        // Use SetScore to update the score
        SetScore(GetScore() + ScoreToAdd);
        
        UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerState: %s score updated to %f"), 
            *GetPlayerName(), 
            GetScore());
    }
}

void ASG_PlayerState::AddKill()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Kills++;
        AddScore(1); // Add to score for each kill
        
        UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerState: %s got a kill (Total: %d)"), 
            *GetPlayerName(), 
            Kills);
    }
}

void ASG_PlayerState::AddDeath()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Deaths++;
        
        UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerState: %s died (Total deaths: %d)"), 
            *GetPlayerName(), 
            Deaths);
    }
}

void ASG_PlayerState::OnRep_TeamId()
{
    // Notify UI or other systems that the team changed
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerState: %s team ID replicated: %d"), 
        *GetPlayerName(), 
        TeamId);
}

void ASG_PlayerState::OnRep_Kills()
{
    // Notify UI or other systems that kills were updated
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerState: %s kills replicated: %d"), 
        *GetPlayerName(), 
        Kills);
}

void ASG_PlayerState::OnRep_Deaths()
{
    // Notify UI or other systems that deaths were updated
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerState: %s deaths replicated: %d"), 
        *GetPlayerName(), 
        Deaths);
}
