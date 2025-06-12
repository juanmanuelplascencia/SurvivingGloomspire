// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Game/SG_GameInstance.h"
#include "SG_LogChannels.h"

USG_GameInstance::USG_GameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Constructor
}

void USG_GameInstance::Init()
{
    Super::Init();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Initializing game instance"));
    
    // Initialize game systems
    InitializeGameSystems();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Game instance initialized"));
}

void USG_GameInstance::Shutdown()
{
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Shutting down game instance"));
    
    // Shutdown game systems
    ShutdownGameSystems();
    
    Super::Shutdown();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Game instance shutdown complete"));
}

void USG_GameInstance::OnStart()
{
    Super::OnStart();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Game started"));
}

void USG_GameInstance::InitializeGameSystems()
{
    // Initialize core game systems here
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Initializing game systems"));
    
    // TODO: Initialize core systems (Event System, Config System, etc.)
}

void USG_GameInstance::ShutdownGameSystems()
{
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SG_GameInstance: Shutting down game systems"));
    
    // TODO: Shutdown core systems in reverse order of initialization
}
