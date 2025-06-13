// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISG_LoggingSystem.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "Containers/Array.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformStackWalk.h"
#include "Misc/ScopeLock.h"
#include "Misc/OutputDevice.h"
#include "Templates/Atomic.h"
#include "Templates/UniquePtr.h"
#include "HAL/PlatformMisc.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/ThreadSafeCounter.h"
#include "HAL/PlatformStackWalk.h"	
#include "HAL/FileManager.h"
#include "HAL/PlatformFile.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformStackWalk.h"
#include "Misc/FeedbackContext.h"
#include "Misc/App.h"
#include "Misc/ConfigCacheIni.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/Class.h"
#include "UObject/EnumProperty.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"

// Forward declarations
class FSG_LoggingSystem;
struct SG_CRASH_CONTEXT;

// Global instance
extern SURVIVINGGLOOMSPIRE_API TUniquePtr<FSG_LoggingSystem> GLoggingSystem;

/**
 * Implementation of the logging system
 */
class FSG_LoggingSystem : public ISG_LoggingSystem
{
public:
    FSG_LoggingSystem();
    virtual ~FSG_LoggingSystem() override;
    void Shutdown();

    // ISG_LoggingSystem interface
    virtual void Log(const FString& Message, ESG_LogVerbosity Verbosity, 
                    const FString& Category, ESG_LogOutputTarget OutputTarget) const override;
    
    virtual void LogWithContext(const FString& Message, const UObject* ContextObject, 
                               ESG_LogVerbosity Verbosity, ESG_LogOutputTarget OutputTarget) const override;
    
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
    
    // ISG_LoggingSystem interface - Crash Reporting
    virtual void InitializeCrashReporting() override;
    virtual void ShutdownCrashReporting() override;
    virtual void SetCrashReportPath(const FString& InCrashReportPath) override;
    virtual void SetCrashReportCallback(FCrashReportCallback InCallback) override;
    virtual void GenerateCrashReport(const FString& ErrorMessage, const ANSICHAR* Function = nullptr, 
                                   const ANSICHAR* File = nullptr, int32 Line = 0, uint32_t ErrorCode = 0) override;
    virtual void HandleCrash(const TCHAR* ErrorMessage, const FString& Callstack) override;

private:
    // Crash reporting
    FString CrashReportPath;
    FCrashReportCallback CrashCallback;
    FThreadSafeBool bCrashReportingInitialized;
    FCriticalSection CrashMutex;
    TArray<FString> PendingCrashReports;
    
    // Crash reporting functions
    void WriteCrashReport(const SG_CRASH_CONTEXT& Context);
    void WriteCrashReportToFile(const FString& Report, const FString& Filename);
    void OnCrashHandler(const FGenericCrashContext& Context);
    static void StaticCrashHandler(const FGenericCrashContext& Context);
    static void OnCrashingThreadSet();
    
    // System error handlers
    void OnSystemError() const;
    void OnSystemEnsure() const;
    
    // Error report handler
    void OnCrashedSendErrorReport();
    
    // Thread safety (must be initialized first)
    FThreadSafeBool bIsInitialized;
    mutable FThreadSafeCounter ActiveLogs;
    
    // Configuration (needs thread safety)
    TMap<FString, ESG_LogVerbosity> CategoryVerbosity;
    ESG_LogVerbosity GlobalVerbosity;
    mutable FCriticalSection ConfigMutex;
    
    // Log file management (needs thread safety)
    FString LogFilePath;
    FArchive* LogFile;
    int64 MaxLogFileSize;
    int32 MaxBackupFiles;
    mutable FCriticalSection FileMutex;
    
    // Timer tracking (needs thread safety)
    TMap<FString, double> ActiveTimers;
    FCriticalSection TimerMutex;
    
    // Memory tracking (needs thread safety)
    TMap<FString, SIZE_T> MemoryTracking;
    FCriticalSection MemoryMutex;
    ;
    
    // Static callbacks
    static void OnCrashingThreadSetStatic();
    static void OnCrashedSendErrorReportStatic();
    
    // Internal logging function
    void InternalLog(const FString& Message, ESG_LogVerbosity Verbosity, const FString& Category, 
                    ESG_LogOutputTarget OutputTarget, const FString& Context = FString()) const;
    
    // File logging functions
    /**
     * Initializes the log file
     * @return true if initialization was successful, false otherwise
     */
    bool InitializeLogFile();
    
    /**
     * Rotates the current log file by creating a backup and starting a new one
     */
    void RotateLogFile();
    
    /**
     * Safely closes the current log file if it's open
     */
    void CloseLogFile();
};
