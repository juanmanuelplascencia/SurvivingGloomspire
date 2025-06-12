// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SurvivingGloomspire.h"
#include "Modules/ModuleManager.h"
#include "Core/SG_LogChannels.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSurvivingGloomspireModule, SurvivingGloomspire, "SurvivingGloomspire");

#define LOCTEXT_NAMESPACE "FSurvivingGloomspireModule"

void FSurvivingGloomspireModule::StartupModule()
{
    // Register module-specific log categories
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module starting up"));
    
    // Register any module-specific settings here
    // RegisterSettings();
    
    // Register any module-specific asset types here
    // RegisterAssetTypes();
    
    // Initialize any module-specific systems here
    // InitializeSystems();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module startup complete"));
}

void FSurvivingGloomspireModule::ShutdownModule()
{
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("Shutting down SurvivingGloomspire module"));
    
    // Shutdown any module-specific systems here
    // ShutdownSystems();
    
    // Unregister any module-specific asset types here
    // UnregisterAssetTypes();
    
    // Unregister any module-specific settings here
    // UnregisterSettings();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module shutdown complete"));
}

#undef LOCTEXT_NAMESPACE
