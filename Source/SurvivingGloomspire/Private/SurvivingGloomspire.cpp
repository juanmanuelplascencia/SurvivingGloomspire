// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SurvivingGloomspire.h"
#include "HAL/PlatformMisc.h"
#include "Core/SG_LogChannels.h"
#include "Core/ISG_LoggingSystem.h"
#include "Misc/OutputDeviceRedirector.h"  // Needed for GLog

IMPLEMENT_PRIMARY_GAME_MODULE(FSurvivingGloomspireModule, SurvivingGloomspire, "SurvivingGloomspire");

#define LOCTEXT_NAMESPACE "FSurvivingGloomspireModule"

// Console command to test crash reporting
static void TestCrashCommand(const TArray<FString>& Args)
{
    if (Args.Num() > 0 && Args[0] == "crash")
    {
        // Trigger a crash
        int* Ptr = nullptr;
        *Ptr = 1; // This will cause a crash
    }
    else if (Args.Num() > 0 && Args[0] == "report")
    {
        // Generate a test crash report
        ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();
        LoggingSystem.GenerateCrashReport(
            TEXT("Test crash report"),
            __FUNCTION__,
            __FILE__,
            __LINE__,
            0xDEADBEEF
        );
        UE_LOG(LogTemp, Log, TEXT("Test crash report generated"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Usage: TestCrash [crash|report]"));
    }
}

static FAutoConsoleCommand TestCrashCommandHandler(
    TEXT("TestCrash"),
    TEXT("Test the crash reporting system (usage: TestCrash [crash|report])"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCrashCommand)
);

void FSurvivingGloomspireModule::StartupModule()
{
    // Register module-specific log categories
    UE_LOG(LogSurvivingGloomspire, Log, TEXT("SurvivingGloomspire module starting up"));
    
    // Initialize the logging system
    ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();
    
    // Initialize crash reporting
    LoggingSystem.InitializeCrashReporting();
    
    // Set a crash report path to Logs/CrashReports
    FString CrashReportPath = FPaths::ProjectLogDir() / TEXT("CrashReports");
    LoggingSystem.SetCrashReportPath(CrashReportPath);
    
    // Configure logging system
    LoggingSystem.SetGlobalVerbosity(ESG_LogVerbosity::Log);
    LoggingSystem.SetMaxLogFileSize(10 * 1024 * 1024); // 10MB
    LoggingSystem.SetMaxBackupFiles(5);
    
    // Log system information
    LoggingSystem.Log("Logging system initialized", ESG_LogVerbosity::Log, "System");
    LoggingSystem.Log("SurvivingGloomspire module startup complete", ESG_LogVerbosity::Log, "System");
    
    // Set up crash reporting
    LoggingSystem.SetCrashReportCallback([](const SG_CRASH_CONTEXT& Context, FOutputDevice& Ar)
    {
        Ar.Logf(TEXT("=== CUSTOM CRASH HANDLER ===\n"));
        Ar.Logf(TEXT("Error: %s\n"), Context.ErrorMessage);
        if (Context.Function && *Context.Function)
        {
            Ar.Logf(TEXT("Function: %s\n"), ANSI_TO_TCHAR(Context.Function));
        }
        if (Context.File && *Context.File)
        {
            Ar.Logf(TEXT("File: %s (Line: %d)\n"), ANSI_TO_TCHAR(Context.File), Context.Line);
        }
        if (Context.ErrorCode != 0)
        {
            Ar.Logf(TEXT("Error Code: 0x%08X\n"), Context.ErrorCode);
        }
        if (!Context.ContextInfo.IsEmpty())
        {
            Ar.Logf(TEXT("\nContext:\n%s\n"), *Context.ContextInfo);
        }
        Ar.Logf(TEXT("=== END CUSTOM CRASH HANDLER ===\n"));
    });
    
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
