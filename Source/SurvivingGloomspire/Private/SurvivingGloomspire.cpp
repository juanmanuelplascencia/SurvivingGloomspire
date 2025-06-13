// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SurvivingGloomspire.h"
#include "Modules/ModuleManager.h"
#include "Core/SG_LogChannels.h"
#include "Core/ISG_LoggingSystem.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSurvivingGloomspireModule, SurvivingGloomspire, "SurvivingGloomspire");

#define LOCTEXT_NAMESPACE "FSurvivingGloomspireModule"

void FSurvivingGloomspireModule::StartupModule()
{
    // Register module-specific log categories
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module starting up"));
    
    // Initialize the logging system
    ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();
    LoggingSystem.Log("Logging system initialized", ESG_LogVerbosity::Log, "System");
    
    // Configure logging system
    LoggingSystem.SetGlobalVerbosity(ESG_LogVerbosity::Log);
    LoggingSystem.SetMaxLogFileSize(10 * 1024 * 1024); // 10MB
    LoggingSystem.SetMaxBackupFiles(5);
    
    // Log system information
    LoggingSystem.Log("SurvivingGloomspire module startup complete", ESG_LogVerbosity::Log, "System");
    
    // Register any module-specific settings here
    // RegisterSettings();
    
    // Register any module-specific asset types here
    // RegisterAssetTypes();
    
    // Initialize any module-specific systems here
    // InitializeSystems();
}

void FSurvivingGloomspireModule::ShutdownModule()
{
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("Shutting down SurvivingGloomspire module"));
    
    // Log shutdown message
    ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();
    LoggingSystem.Log("Shutting down SurvivingGloomspire module", ESG_LogVerbosity::Log, "System");
    LoggingSystem.FlushLogs();
    
    // Shutdown any module-specific systems here
    // ShutdownSystems();
    
    // Unregister any module-specific asset types here
    // UnregisterAssetTypes();
    
    // Unregister any module-specific settings here
    // UnregisterSettings();
    
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module shutdown complete"));
}

#undef LOCTEXT_NAMESPACE
