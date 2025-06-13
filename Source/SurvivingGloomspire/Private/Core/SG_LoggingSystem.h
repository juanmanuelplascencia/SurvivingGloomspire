// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISG_LoggingSystem.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "Misc/ScopeLock.h"
#include "Templates/Atomic.h"
#include "Templates/UniquePtr.h"

// Forward declarations
class FSG_LoggingSystem;

// Global instance
extern SURVIVINGGLOOMSPIRE_API TUniquePtr<FSG_LoggingSystem> GLoggingSystem;

/**
 * Implementation of the logging system
 */
class FSG_LoggingSystem : public ISG_LoggingSystem
{
public:
    FSG_LoggingSystem();
    ~FSG_LoggingSystem();
    void Shutdown();

    // ISG_LoggingSystem interface
    virtual void Log(const FString& Message, ESG_LogVerbosity Verbosity, 
                    const FString& Category, ESG_LogOutputTarget OutputTarget) override;
    
    virtual void LogWithContext(const FString& Message, const UObject* ContextObject, 
                               ESG_LogVerbosity Verbosity, ESG_LogOutputTarget OutputTarget) override;
    
    virtual void StartTimer(const FString& TimerName) override;
    virtual double StopTimer(const FString& TimerName, const FString& Message) override;
    
    virtual void TrackMemory(const FString& Tag) override;
    virtual void UntrackMemory(const FString& Tag) override;
    
    virtual void SetLogFile(const FString& FilePath) override;
    virtual void SetMaxLogFileSize(int64 MaxSizeInBytes) override;
    virtual void SetMaxBackupFiles(int32 MaxFiles) override;
    virtual void FlushLogs() override;
    
    virtual void SetGlobalVerbosity(ESG_LogVerbosity Verbosity) override;
    virtual void SetCategoryVerbosity(const FString& Category, ESG_LogVerbosity Verbosity) override;

private:
    // Thread safety (must be initialized first)
    FThreadSafeBool bIsInitialized;
    FThreadSafeCounter ActiveLogs;
    
    // Configuration (needs thread safety)
    TMap<FString, ESG_LogVerbosity> CategoryVerbosity;
    ESG_LogVerbosity GlobalVerbosity;
    FCriticalSection ConfigMutex;
    
    // Log file management (needs thread safety)
    FString LogFilePath;
    FArchive* LogFile;
    int64 MaxLogFileSize;
    int32 MaxBackupFiles;
    FCriticalSection FileMutex;
    
    // Timer tracking (needs thread safety)
    TMap<FString, double> ActiveTimers;
    FCriticalSection TimerMutex;
    
    // Memory tracking (needs thread safety)
    TMap<FString, SIZE_T> MemoryTracking;
    FCriticalSection MemoryMutex;
    
    // Internal logging function
    void InternalLog(const FString& Message, ESG_LogVerbosity Verbosity, const FString& Category, 
                    ESG_LogOutputTarget OutputTarget, const FString& Context = FString());
    
    // File logging functions
    void InitializeLogFile();
    void RotateLogFile();
};
