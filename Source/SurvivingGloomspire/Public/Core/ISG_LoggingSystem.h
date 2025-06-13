// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISG_LoggingSystem.generated.h"

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
                    ESG_LogOutputTarget OutputTarget = ESG_LogOutputTarget::All) = 0;

    // Log with context (useful for tracing object-specific logs)
    virtual void LogWithContext(const FString& Message, const UObject* ContextObject, 
                               ESG_LogVerbosity Verbosity = ESG_LogVerbosity::Log, 
                               ESG_LogOutputTarget OutputTarget = ESG_LogOutputTarget::All) = 0;

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
};

// Global accessor for the logging system
SURVIVINGGLOOMSPIRE_API ISG_LoggingSystem& GetSGLoggingSystem();
