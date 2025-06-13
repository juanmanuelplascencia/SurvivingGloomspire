// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Core/SG_LoggingSystem.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformMisc.h"
#include "HAL/PlatformStackWalk.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/Class.h"
#include "Misc/CoreDelegates.h"

// Global instance
TUniquePtr<FSG_LoggingSystem> GLoggingSystem;

// Static callback for crash handling
void FSG_LoggingSystem::StaticCrashHandler(const FGenericCrashContext& Context)
{
    if (GLoggingSystem.IsValid())
    {
        GLoggingSystem->OnCrashHandler(Context);
    }
}

// Static callback for a crash reporting thread
void FSG_LoggingSystem::OnCrashingThreadSetStatic()
{
    if (GLoggingSystem.IsValid())
    {
        GLoggingSystem->OnCrashingThreadSet();
    }
}

// Static callback for sending error reports
void FSG_LoggingSystem::OnCrashedSendErrorReportStatic()
{
    if (GLoggingSystem.IsValid())
    {
        GLoggingSystem->OnCrashedSendErrorReport();
    }
}

FSG_LoggingSystem::FSG_LoggingSystem()
    : CrashReportPath()
    , CrashCallback(nullptr)
    , bCrashReportingInitialized(false)
    , CrashMutex()
    , PendingCrashReports()
    , bIsInitialized(false)
    , ActiveLogs(0)
    , CategoryVerbosity()
    , GlobalVerbosity(ESG_LogVerbosity::Log)
    , ConfigMutex()
    , LogFilePath(FPaths::ProjectLogDir() / TEXT("SG_Game.log"))
    , LogFile(nullptr)
    , MaxLogFileSize(10 * 1024 * 1024) // 10MB default max size
    , MaxBackupFiles(5)
{
    // Set up default category verbosity
    CategoryVerbosity.Add(TEXT("Game"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("Gameplay"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("AI"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("UI"), ESG_LogVerbosity::Log);
    
    // Initialize log file
    if (InitializeLogFile())
    {
        bIsInitialized = true;
    }
}

FSG_LoggingSystem::~FSG_LoggingSystem()
{
    // Ensure proper cleanup even if Shutdown wasn't called
    if (bIsInitialized)
    {
        // Explicitly flush any pending logs to file
        if (LogFile != nullptr)
        {
            LogFile->Flush();
        }
        
        // Close the log file
        CloseLogFile();
        
        // Clear any pending logs
        ActiveLogs.Reset();
        bIsInitialized = false;
    }
    
    // Clean up crash reporting if it was initialized
    if (bCrashReportingInitialized)
    {
        // Remove all our registered delegates
        FCoreDelegates::OnHandleSystemError.RemoveAll(this);
        FCoreDelegates::OnHandleSystemEnsure.RemoveAll(this);
        
        // Clear the crash handler
        FPlatformMisc::SetCrashHandler(nullptr);
        bCrashReportingInitialized = false;
    }
    
    // Clear any pending crash reports
    {
        FScopeLock CrashLock(&CrashMutex);
        PendingCrashReports.Empty();
    }
    
    // Clear any remaining timers and memory tracking
    {
        FScopeLock TimerLock(&TimerMutex);
        ActiveTimers.Empty();
    }
    
    {
        FScopeLock MemoryLock(&MemoryMutex);
        MemoryTracking.Empty();
    }
    
    // Clear category verbosity
    {
        FScopeLock ConfigLock(&ConfigMutex);
        CategoryVerbosity.Empty();
    }
}

void FSG_LoggingSystem::Shutdown()
{
    // Early out if already shut down
    if (!bIsInitialized)
    {
        return;
    }
    
    // Flush and close the log file
    FlushLogs();
    CloseLogFile();
    
    // Clear any pending logs
    ActiveLogs.Reset();
    
    // Shutdown crash reporting
    ShutdownCrashReporting();
    
    // Clear all containers to free memory
    {
        FScopeLock TimerLock(&TimerMutex);
        ActiveTimers.Empty();
    }
    
    {
        FScopeLock MemoryLock(&MemoryMutex);
        MemoryTracking.Empty();
    }
    
    {
        FScopeLock ConfigLock(&ConfigMutex);
        CategoryVerbosity.Empty();
    }
    
    // Mark as uninitialized
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("Logging system shutdown complete"));
}

void FSG_LoggingSystem::Log(const FString& Message, ESG_LogVerbosity Verbosity, 
                          const FString& Category, ESG_LogOutputTarget OutputTarget) const
{
    if (!bIsInitialized) return;
    
    ActiveLogs.Increment();
    InternalLog(Message, Verbosity, Category, OutputTarget);
    ActiveLogs.Decrement();
}

void FSG_LoggingSystem::LogWithContext(const FString& Message, const UObject* ContextObject, 
                                     ESG_LogVerbosity Verbosity, ESG_LogOutputTarget OutputTarget) const
{
    if (!bIsInitialized) return;
    
    FString Context = ContextObject ? 
        FString::Printf(TEXT("%s [%s]"), *ContextObject->GetName(), *ContextObject->GetClass()->GetName()) : 
        TEXT("None");
    
    ActiveLogs.Increment();
    InternalLog(Message, Verbosity, TEXT("Game"), OutputTarget, Context);
    ActiveLogs.Decrement();
}

void FSG_LoggingSystem::InternalLog(const FString& Message, ESG_LogVerbosity Verbosity, 
                                  const FString& Category, ESG_LogOutputTarget OutputTarget,
                                  const FString& Context) const
{
    // Early out if not initialized
    if (!bIsInitialized)
    {
        return;
    }
    
    // Check verbosity level
    {
        FScopeLock ConfigLock(&ConfigMutex);
        const ESG_LogVerbosity* CategoryVerbosityPtr = CategoryVerbosity.Find(Category);
        const ESG_LogVerbosity CurrentVerbosity = CategoryVerbosityPtr ? *CategoryVerbosityPtr : GlobalVerbosity;
        
        if (Verbosity > CurrentVerbosity)
        {
            return; // Skip if below the verbosity threshold
        }
    }
    
    // Format the log message
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y.%m.%d-%H:%M:%S"));
    
    // Get enum as a string
    const UEnum* EnumPtr = StaticEnum<ESG_LogVerbosity>();
    FString VerbosityString = EnumPtr ? 
        EnumPtr->GetNameStringByValue(static_cast<int64>(Verbosity)) : 
        FString::Printf(TEXT("%d"), static_cast<int32>(Verbosity));
    
    FString LogMessage = FString::Printf(TEXT("[%s] [%s] [%s] %s"), 
        *Timestamp, 
        *VerbosityString, 
        *Category, 
        *Message);
        
    if (!Context.IsEmpty())
    {
        LogMessage += FString::Printf(TEXT(" | %s"), *Context);
    }
    
    LogMessage += TEXT("\n");
    
    // Output to file target if enabled
    if (EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::File))
    {
        FScopeLock FileLock(&FileMutex);
        
        // Ensure we have a valid log file
        if (!LogFile && !LogFilePath.IsEmpty())
        {
            // Try to reinitialize the log file if it was closed unexpectedly
            const_cast<FSG_LoggingSystem*>(this)->InitializeLogFile();
        }
        
        if (LogFile)
        {
            // Check if we need to rotate the log file before writing
            const int64 CurrentPosition = LogFile->Tell();
            if (CurrentPosition >= 0 && (CurrentPosition + LogMessage.Len()) >= MaxLogFileSize)
            {
                // Rotate the log file if we're about to exceed the max size
                const_cast<FSG_LoggingSystem*>(this)->RotateLogFile();
            }
            
            // Write the log message
            LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
            
            // Flush on error or fatal messages to ensure they're written
            if (Verbosity <= ESG_LogVerbosity::Error)
            {
                LogFile->Flush();
            }
        }
    }
    
    // Output to console target if enabled
    if (EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::Console))
    {
        FString ConsoleMessage = FString::Printf(TEXT("[%s] %s"), *Category, *Message);
        
        switch (Verbosity)
        {
            case ESG_LogVerbosity::Fatal:
                UE_LOG(LogTemp, Fatal, TEXT("%s"), *ConsoleMessage);
                break;
            case ESG_LogVerbosity::Error:
                UE_LOG(LogTemp, Error, TEXT("%s"), *ConsoleMessage);
                break;
            case ESG_LogVerbosity::Warning:
                UE_LOG(LogTemp, Warning, TEXT("%s"), *ConsoleMessage);
                break;
            case ESG_LogVerbosity::Display:
                UE_LOG(LogTemp, Display, TEXT("%s"), *ConsoleMessage);
                break;
            case ESG_LogVerbosity::Log:
            default:
                UE_LOG(LogTemp, Log, TEXT("%s"), *ConsoleMessage);
                break;
        }
    }
    
    // Output to screen target if enabled and we have a valid engine
    if (GEngine && EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::Screen))
    {
        FColor TextColor;
        float DisplayTime = 5.0f; // Default display time in seconds
        
        switch (Verbosity)
        {
            case ESG_LogVerbosity::Fatal:
                TextColor = FColor::Red;
                DisplayTime = 15.0f; // Show fatal errors longer
                break;
            case ESG_LogVerbosity::Error:
                TextColor = FColor::Red;
                DisplayTime = 10.0f; // Show errors longer than normal
                break;
            case ESG_LogVerbosity::Warning:
                TextColor = FColor::Yellow;
                DisplayTime = 7.0f; // Show warnings a bit longer than normal
                break;
            case ESG_LogVerbosity::Display:
                TextColor = FColor::Cyan;
                break;
            case ESG_LogVerbosity::Log:
            default:
                TextColor = FColor::White;
                break;
        }
        
        // Add the message to the screen
        GEngine->AddOnScreenDebugMessage(
            -1,                   // Key (or -1 for no key)
            DisplayTime,           // Time to display
            TextColor,             // Text color
            LogMessage,            // Message text
            false,                 // Enable shadow
            FVector2D(1.0f, 1.0f) // Scale
        );
    }
}

// Implementation of timer functions
void FSG_LoggingSystem::StartTimer(const FString& TimerName)
{
    FScopeLock Lock(&TimerMutex);
    ActiveTimers.Add(TimerName, FPlatformTime::Seconds());
}

double FSG_LoggingSystem::StopTimer(const FString& TimerName, const FString& Message)
{
    FScopeLock Lock(&TimerMutex);
    
    const double* StartTime = ActiveTimers.Find(TimerName);
    if (!StartTime)
    {
        Log(FString::Printf(TEXT("Timer '%s' was never started!"), *TimerName), 
            ESG_LogVerbosity::Warning, TEXT("Performance"), ESG_LogOutputTarget::All);
        return 0.0;
    }
    
    const double EndTime = FPlatformTime::Seconds();
    const double Duration = (EndTime - *StartTime) * 1000.0; // Convert to milliseconds
    
    FString LogMessage = FString::Printf(TEXT("Timer '%s' took %.2fms"), *TimerName, Duration);
    if (!Message.IsEmpty())
    {
        LogMessage += FString::Printf(TEXT(": %s"), *Message);
    }
    
    Log(LogMessage, ESG_LogVerbosity::Log, TEXT("Performance"), ESG_LogOutputTarget::All);
    ActiveTimers.Remove(TimerName);
    
    return Duration;
}

// Memory tracking functions
void FSG_LoggingSystem::TrackMemory(const FString& Tag)
{
    FScopeLock Lock(&MemoryMutex);
    
    SIZE_T CurrentMemory = FPlatformMemory::GetStats().UsedPhysical;
    if (const SIZE_T* PreviousMemory = MemoryTracking.Find(Tag))
    {
        const int64 Difference = static_cast<int64>(CurrentMemory) - static_cast<int64>(*PreviousMemory);
        Log(FString::Printf(TEXT("Memory '%s' changed by %lld bytes (%.2f MB)"), 
            *Tag, Difference, Difference / (1024.0f * 1024.0f)), 
            ESG_LogVerbosity::Log, TEXT("Memory"), ESG_LogOutputTarget::All);
    }
    
    MemoryTracking.Add(Tag, CurrentMemory);
}

void FSG_LoggingSystem::UntrackMemory(const FString& Tag)
{
    FScopeLock Lock(&MemoryMutex);
    MemoryTracking.Remove(Tag);
}

// File management functions
bool FSG_LoggingSystem::InitializeLogFile()
{
    FScopeLock FileLock(&FileMutex);
    
    // Close any existing log file
    CloseLogFile();
    
    // Ensure we have a valid log file path
    if (LogFilePath.IsEmpty())
    {
        LogFilePath = FPaths::ProjectLogDir() / TEXT("SG_Game.log");
    }
    
    // Create the log directory if it doesn't exist
    const FString Directory = FPaths::GetPath(LogFilePath);
    if (!IFileManager::Get().MakeDirectory(*Directory, true) && !IFileManager::Get().DirectoryExists(*Directory))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create log directory: %s"), *Directory);
        return false;
    }
    
    // Ensure the log file can be created and written to
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    IFileHandle* TestFile = PlatformFile.OpenWrite(*LogFilePath, true, false);
    if (!TestFile)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot write to log file: %s"), *LogFilePath);
        return false;
    }
    delete TestFile;
    
    // Open the log file for appending
    LogFile = IFileManager::Get().CreateFileWriter(*LogFilePath, FILEWRITE_Append);
    if (!LogFile)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create log file writer for: %s"), *LogFilePath);
        return false;
    }
    
    // Write header to the log file
    const FString Header = FString::Printf(
        TEXT("\n=== SG Log Started: %s ===\n\n"), 
        *FDateTime::Now().ToString(TEXT("%Y.%m.%d-%H:%M:%S")));
    
    LogFile->Serialize(TCHAR_TO_ANSI(*Header), Header.Len());
    LogFile->Flush();
    
    return true;
}

void FSG_LoggingSystem::RotateLogFile()
{
    FScopeLock FileLock(&FileMutex);
    
    if (!LogFile || LogFilePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot rotate log: No active log file"));
        return;
    }
    
    // Get the current log file size
    const int64 CurrentFileSize = IFileManager::Get().FileSize(*LogFilePath);
    if (CurrentFileSize < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get log file size for rotation"));
        return;
    }
    
    // Only rotate if we've exceeded the max file size
    if (CurrentFileSize < MaxLogFileSize)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Rotating log file (current size: %lld bytes)"), CurrentFileSize);
    
    // Close the current log file
    CloseLogFile();
    
    // Create a backup directory if it doesn't exist
    const FString BackupDir = FPaths::GetPath(LogFilePath) / TEXT("Backup");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.CreateDirectoryTree(*BackupDir))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create backup directory: %s"), *BackupDir);
        // Try to continue with the original directory
    }
    
    // Generate timestamp for the backup file
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    const FString BackupFilename = FPaths::GetBaseFilename(LogFilePath) + 
                                 TEXT("_") + Timestamp + 
                                 FPaths::GetExtension(LogFilePath, true);
    const FString BackupPath = BackupDir / BackupFilename;
    
    // Move current log to back-up location
    if (!IFileManager::Get().Move(*BackupPath, *LogFilePath, true, true, true))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to move log file to backup location: %s"), *BackupPath);
    }
    
    // Reinitialize the log file
    if (!InitializeLogFile())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to reinitialize log file after rotation"));
    }
    
    // Clean up old backups if we have too many
    if (MaxBackupFiles > 0)
    {
        TArray<FString> BackupFiles;
        const FString SearchPattern = FPaths::GetBaseFilename(LogFilePath) + TEXT("_*") + FPaths::GetExtension(LogFilePath, true);
        const FString SearchDir = FPaths::GetPath(BackupPath);
        
        // Find all backup files
        IFileManager::Get().FindFiles(BackupFiles, *FPaths::Combine(SearchDir, SearchPattern), true, false);
        
        if (BackupFiles.Num() > MaxBackupFiles)
        {
            // Sort by creation time (oldest first)
            BackupFiles.Sort([&SearchDir](const FString& A, const FString& B) {
                return IFileManager::Get().GetTimeStamp(*(SearchDir / A)) < 
                       IFileManager::Get().GetTimeStamp(*(SearchDir / B));
            });
            
            // Delete the oldest files
            const int32 FilesToRemove = BackupFiles.Num() - MaxBackupFiles;
            for (int32 i = 0; i < FilesToRemove; ++i)
            {
                const FString FileToRemove = SearchDir / BackupFiles[i];
                if (!IFileManager::Get().Delete(*FileToRemove, false, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to delete old log file: %s"), *FileToRemove);
                }
            }
        }
    }
}

void FSG_LoggingSystem::CloseLogFile()
{
    FScopeLock FileLock(&FileMutex);
    
    if (!LogFile)
    {
        return; // Already closed or never opened
    }
    
    // Ensure all data is written to the disk
    LogFile->Flush();
    
    // Close and delete the file handle
    LogFile->Close();
    delete LogFile;
    LogFile = nullptr;
    
    // Verify the file was properly closed
    if (IFileManager::Get().FileExists(*LogFilePath))
    {
        // Check if we can open the file for exclusive access
        IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
        IFileHandle* TestHandle = PlatformFile.OpenRead(*LogFilePath);
        if (TestHandle)
        {
            // File is still locked by another process
            delete TestHandle;
            UE_LOG(LogTemp, Warning, TEXT("Log file still locked after closing: %s"), *LogFilePath);
        }
    }
}

void FSG_LoggingSystem::SetLogFile(const FString& FilePath)
{
    FScopeLock FileLock(&FileMutex);
    
    // Close the current log file if open
    CloseLogFile();
    
    // Set a new log file path and initialize
    LogFilePath = FilePath;
    InitializeLogFile();
}

void FSG_LoggingSystem::SetMaxLogFileSize(int64 MaxSizeInBytes)
{
    FScopeLock Lock(&FileMutex);
    MaxLogFileSize = FMath::Max(1024LL, MaxSizeInBytes); // Minimum 1KB
}

void FSG_LoggingSystem::SetMaxBackupFiles(int32 MaxFiles)
{
    FScopeLock Lock(&FileMutex);
    MaxBackupFiles = FMath::Max(0, MaxFiles);
}

void FSG_LoggingSystem::FlushLogs()
{
    FScopeLock Lock(&FileMutex);
    if (LogFile)
    {
        LogFile->Flush();
    }
}

void FSG_LoggingSystem::SetGlobalVerbosity(ESG_LogVerbosity Verbosity)
{
    FScopeLock Lock(&ConfigMutex);
    GlobalVerbosity = Verbosity;
}

void FSG_LoggingSystem::SetCategoryVerbosity(const FString& Category, ESG_LogVerbosity Verbosity)
{
    FScopeLock Lock(&ConfigMutex);
    CategoryVerbosity.Add(Category, Verbosity);
}

// Crash Reporting Implementation
void FSG_LoggingSystem::InitializeCrashReporting()
{
    FScopeLock Lock(&CrashMutex);
    if (bCrashReportingInitialized) 
    {
        return;
    }
    
    // Create a crash report directory
    if (!IFileManager::Get().MakeDirectory(*CrashReportPath, true))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create crash report directory: %s"), *CrashReportPath);
    }
    
    // Set up a crash handler
    FPlatformMisc::SetCrashHandler(StaticCrashHandler);
    FPlatformMisc::SetGracefulTerminationHandler();
    
    // Register error handlers
    FCoreDelegates::OnHandleSystemError.AddRaw(this, &FSG_LoggingSystem::OnSystemError);
    FCoreDelegates::OnHandleSystemEnsure.AddRaw(this, &FSG_LoggingSystem::OnSystemEnsure);
    
    bCrashReportingInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("Crash reporting initialized"));
    Log(TEXT("Crash reporting initialized"), ESG_LogVerbosity::Log, TEXT("System"), ESG_LogOutputTarget::All);
}

void FSG_LoggingSystem::ShutdownCrashReporting()
{
    FScopeLock Lock(&CrashMutex);
    
    if (!bCrashReportingInitialized)
    {
        return;
    }
    
    // Remove our error handlers
    FCoreDelegates::OnHandleSystemError.RemoveAll(this);
    FCoreDelegates::OnHandleSystemEnsure.RemoveAll(this);
    
    // Clear the crash handler
    FPlatformMisc::SetCrashHandler(nullptr);
    
    bCrashReportingInitialized = false;
    
    Log(TEXT("Crash reporting shutdown"), ESG_LogVerbosity::Log, TEXT("System"), ESG_LogOutputTarget::All);
}

void FSG_LoggingSystem::SetCrashReportPath(const FString& InCrashReportPath)
{
    FScopeLock Lock(&CrashMutex);
    CrashReportPath = InCrashReportPath;
    IFileManager::Get().MakeDirectory(*CrashReportPath, true);
}

void FSG_LoggingSystem::SetCrashReportCallback(FCrashReportCallback InCallback)
{
    FScopeLock Lock(&CrashMutex);
    CrashCallback = InCallback;
}

void FSG_LoggingSystem::GenerateCrashReport(const FString& ErrorMessage, const ANSICHAR* Function, 
                                          const ANSICHAR* File, int32 Line, uint32_t ErrorCode)
{
    SG_CRASH_CONTEXT Context;
    Context.ErrorMessage = *ErrorMessage;
    Context.Function = Function;
    Context.File = File;
    Context.Line = Line;
    Context.ErrorCode = ErrorCode;
    Context.Address = PLATFORM_RETURN_ADDRESS();
    
    // Get callstack using ANSICHAR buffer
    ANSICHAR StackTrace[65536] = {0};
    FPlatformStackWalk::StackWalkAndDump(StackTrace, sizeof(StackTrace) - 1, 0);
    Context.ContextInfo = UTF8_TO_TCHAR(StackTrace);
    
    WriteCrashReport(Context);
}

void FSG_LoggingSystem::HandleCrash(const TCHAR* ErrorMessage, const FString& Callstack)
{
    SG_CRASH_CONTEXT Context;
    Context.ErrorMessage = ErrorMessage;
    Context.Function = "";
    Context.File = "";
    Context.Line = 0;
    Context.ErrorCode = 0;
    Context.Address = PLATFORM_RETURN_ADDRESS();
    Context.ContextInfo = Callstack;
    
    WriteCrashReport(Context);
}

void FSG_LoggingSystem::OnCrashHandler(const FGenericCrashContext& GenericContext)
{
    FScopeLock Lock(&CrashMutex);
    
    // Get callstack using ANSICHAR buffer
    ANSICHAR Callstack[65536] = {0};
    FPlatformStackWalk::StackWalkAndDump(Callstack, sizeof(Callstack) - 1, 0);
    FString CallstackStr = UTF8_TO_TCHAR(Callstack);
    
    // Create crash context
    SG_CRASH_CONTEXT Context;
    Context.ErrorMessage = TEXT("Application crashed");
    Context.Function = "";
    Context.File = "";
    Context.Line = 0;
    Context.ErrorCode = 0;
    Context.Address = nullptr;
    Context.ContextInfo = Callstack;
    
    WriteCrashReport(Context);
}

void FSG_LoggingSystem::OnCrashingThreadSet()
{
    // This is called when the crashing thread is set
    // We can perform any thread-specific initialization here
    FPlatformMisc::SetGracefulTerminationHandler();
}

void FSG_LoggingSystem::OnSystemError() const
{
    // Called when a system error occurs
    FString ErrorMessage = TEXT("System error occurred");
    ANSICHAR Callstack[65536] = {0};
    FPlatformStackWalk::StackWalkAndDump(Callstack, sizeof(Callstack) - 1, 0);
    FString CallstackStr = UTF8_TO_TCHAR(Callstack);
    
    // Log the error
    Log(FString::Printf(TEXT("%s\n%s"), *ErrorMessage, *CallstackStr), 
        ESG_LogVerbosity::Error, TEXT("System"), ESG_LogOutputTarget::All);
}

void FSG_LoggingSystem::OnSystemEnsure() const
{
    // Called when an ensuring is hit
    FString ErrorMessage = TEXT("System ensure hit");
    ANSICHAR Callstack[65536] = {0};
    FPlatformStackWalk::StackWalkAndDump(Callstack, sizeof(Callstack) - 1, 0);
    FString CallstackStr = UTF8_TO_TCHAR(Callstack);
    
    // Log the ensuring
    Log(FString::Printf(TEXT("%s\n%s"), *ErrorMessage, *CallstackStr), 
        ESG_LogVerbosity::Warning, TEXT("System"), ESG_LogOutputTarget::All);
}

void FSG_LoggingSystem::OnCrashedSendErrorReport()
{
    // This is called when we're about to send an error report,
    // We can add any additional information to the report here
}

void FSG_LoggingSystem::WriteCrashReport(const SG_CRASH_CONTEXT& Context)
{
    FScopeLock Lock(&CrashMutex);
    
    if (!bCrashReportingInitialized)
    {
        return;
    }
    
    // Create report content
    FString Report;
    Report += FString::Printf(TEXT("=== Crash Report ===\n"));
    Report += FString::Printf(TEXT("Time: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Error: %s\n"), Context.ErrorMessage);
    
    if (Context.Function && FCStringAnsi::Strlen(Context.Function) > 0)
    {
        Report += FString::Printf(TEXT("Function: %s\n"), ANSI_TO_TCHAR(Context.Function));
    }
    
    if (Context.File && FCStringAnsi::Strlen(Context.File) > 0)
    {
        Report += FString::Printf(TEXT("File: %s (Line: %d)\n"), ANSI_TO_TCHAR(Context.File), Context.Line);
    }
    
    if (Context.ErrorCode != 0)
    {
        Report += FString::Printf(TEXT("Error Code: 0x%08X\n"), Context.ErrorCode);
    }
    
    // Add a callstack if available
    if (!Context.ContextInfo.IsEmpty())
    {
        Report += FString::Printf(TEXT("\n=== Callstack ===\n%s\n"), *Context.ContextInfo);
    }
    
    // Add system info
    Report += FString::Printf(TEXT("\n=== System Info ===\n"));
    Report += FString::Printf(TEXT("OS: %s\n"), *FPlatformMisc::GetOSVersion());
    Report += FString::Printf(TEXT("CPU: %s\n"), *FPlatformMisc::GetCPUVendor());
    
    // Add memory stats
    FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
    Report += FString::Printf(TEXT("\n=== Memory Stats ===\n"));
    Report += FString::Printf(TEXT("Used Physical: %.2f MB\n"), Stats.UsedPhysical / (1024.0f * 1024.0f));
    Report += FString::Printf(TEXT("Used Virtual: %.2f MB\n"), Stats.UsedVirtual / (1024.0f * 1024.0f));
    
    // Create filename with a timestamp
    FString Filename = FString::Printf(TEXT("CrashReport_%s.log"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    
    // Write to a file
    WriteCrashReportToFile(Report, Filename);
    
    // Call callback if set
    if (CrashCallback)
    {
        CrashCallback(Context, *GLog);
    }
    
    // Also log to our normal log
    Log(FString::Printf(TEXT("Crash report generated: %s"), *Filename), ESG_LogVerbosity::Error, TEXT("Crash"), ESG_LogOutputTarget::All);
}

void FSG_LoggingSystem::WriteCrashReportToFile(const FString& Report, const FString& Filename)
{
    FString FullPath = FPaths::Combine(CrashReportPath, Filename);
    FArchive* Writer = IFileManager::Get().CreateFileWriter(*FullPath, FILEWRITE_NoFail);
    
    if (Writer)
    {
        FTCHARToUTF8 UTF8String(*Report);
        Writer->Serialize((void*)UTF8String.Get(), UTF8String.Length());
        Writer->Close();
        delete Writer;
    }
}

// Global accessor implementation
ISG_LoggingSystem& GetSGLoggingSystem()
{
    if (!GLoggingSystem.IsValid())
    {
        GLoggingSystem = MakeUnique<FSG_LoggingSystem>();
    }
    return *GLoggingSystem;
}
