// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HAL/Platform.h"
#include "Misc/AssertionMacros.h"
#include "ISG_LoggingSystem.generated.h"

// Forward declarations
class FString;
class UObject;
class FOutputDevice;

// Crash context structure
struct SG_CRASH_CONTEXT
{
    const TCHAR* ErrorMessage;
    const ANSICHAR* Function;
    const ANSICHAR* File;
    int32 Line;
    uint32_t ErrorCode;
    const void* Address;
    FString ContextInfo;
};

// Crash report callback type
typedef void(*FCrashReportCallback)(const SG_CRASH_CONTEXT& Context, FOutputDevice& Ar);

// Log verbosity levels mirroring ELogVerbosity::Type but with our own enum for better control
UENUM(BlueprintType)
enum class ESG_LogVerbosity : uint8
{
    Fatal,
    Error,
    Warning,
    Display,
    Log,
    Verbose,
    VeryVerbose
};

// Log output targets
UENUM(BlueprintType)
enum class ESG_LogOutputTarget : uint8
{
    None = 0x00,
    File = 0x01,
    Screen = 0x02,
    Console = 0x04,
    All = File | Screen | Console
};
ENUM_CLASS_FLAGS(ESG_LogOutputTarget);

/**
 * Interface for the game's logging system
 */
class SURVIVINGGLOOMSPIRE_API ISG_LoggingSystem
{
public:
    virtual ~ISG_LoggingSystem() = default;

    // Basic logging
    virtual void Log(const FString& Message, ESG_LogVerbosity Verbosity = ESG_LogVerbosity::Log, 
                    const FString& Category = TEXT("Game"), 
                    ESG_LogOutputTarget OutputTarget = ESG_LogOutputTarget::All) const = 0;

    // Log with context (useful for tracing object-specific logs)
    virtual void LogWithContext(const FString& Message, const UObject* ContextObject, 
                               ESG_LogVerbosity Verbosity = ESG_LogVerbosity::Log, 
                               ESG_LogOutputTarget OutputTarget = ESG_LogOutputTarget::All) const = 0;

    // Performance timing
    virtual void StartTimer(const FString& TimerName) = 0;
    virtual double StopTimer(const FString& TimerName, const FString& Message = TEXT("")) = 0;

    // Memory tracking
    virtual void TrackMemory(const FString& Tag) = 0;
    virtual void UntrackMemory(const FString& Tag) = 0;

    // Configuration
    virtual void SetLogFile(const FString& FilePath) = 0;
    virtual void SetMaxLogFileSize(int64 MaxSizeInBytes) = 0;
    virtual void SetMaxBackupFiles(int32 MaxFiles) = 0;
    virtual void FlushLogs() = 0;

    // Log verbosity control
    virtual void SetGlobalVerbosity(ESG_LogVerbosity Verbosity) = 0;
    virtual void SetCategoryVerbosity(const FString& Category, ESG_LogVerbosity Verbosity) = 0;

    // Crash reporting
    virtual void InitializeCrashReporting() = 0;
    virtual void ShutdownCrashReporting() = 0;
    virtual void SetCrashReportPath(const FString& InCrashReportPath) = 0;
    virtual void SetCrashReportCallback(FCrashReportCallback InCallback) = 0;
    virtual void GenerateCrashReport(const FString& ErrorMessage, const ANSICHAR* Function = nullptr, 
                                   const ANSICHAR* File = nullptr, int32 Line = 0, uint32_t ErrorCode = 0) = 0;
    virtual void HandleCrash(const TCHAR* ErrorMessage, const FString& Callstack) = 0;
};

// Global accessor for the logging system
SURVIVINGGLOOMSPIRE_API ISG_LoggingSystem& GetSGLoggingSystem();
