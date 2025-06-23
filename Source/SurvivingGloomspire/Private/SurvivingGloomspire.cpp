// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SurvivingGloomspire.h"
#include "HAL/PlatformMisc.h"
#include "Misc/OutputDeviceRedirector.h"  // Needed for GLog

IMPLEMENT_PRIMARY_GAME_MODULE(FSurvivingGloomspireModule, SurvivingGloomspire, "SurvivingGloomspire");

#define LOCTEXT_NAMESPACE "FSurvivingGloomspireModule"

void FSurvivingGloomspireModule::StartupModule()
{
    \
    // Register any module-specific settings here
    // RegisterSettings();
    
    // Register any module-specific asset types here
    // RegisterAssetTypes();
    
    // Initialize any module-specific systems here
    // InitializeSystems();
}

void FSurvivingGloomspireModule::ShutdownModule()
{
    
    // Shutdown any module-specific systems here
    // ShutdownSystems();
    
    // Unregister any module-specific asset types here
    // UnregisterAssetTypes();
    
    // Unregister any module-specific settings here
    // UnregisterSettings();
}

#undef LOCTEXT_NAMESPACE
