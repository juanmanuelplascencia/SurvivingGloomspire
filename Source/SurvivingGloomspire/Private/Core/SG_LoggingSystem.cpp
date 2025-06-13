// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Core/SG_LoggingSystem.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/ScopeLock.h"
#include "HAL/PlatformProcess.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/Class.h"
#include "UObject/EnumProperty.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"

// Global instance
TUniquePtr<FSG_LoggingSystem> GLoggingSystem;

FSG_LoggingSystem::FSG_LoggingSystem()
    : bIsInitialized(false)
    , ActiveLogs(0)
    , GlobalVerbosity(ESG_LogVerbosity::Log)
    , LogFilePath(TEXT(""))
    , LogFile(nullptr)
    , MaxLogFileSize(10 * 1024 * 1024) // 10MB default max size
    , MaxBackupFiles(5)
{
    // Set up default category verbosities
    CategoryVerbosity.Add(TEXT("Game"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("Gameplay"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("AI"), ESG_LogVerbosity::Log);
    CategoryVerbosity.Add(TEXT("UI"), ESG_LogVerbosity::Log);
    
    // Initialize log file
    InitializeLogFile();
    bIsInitialized = true;
}

FSG_LoggingSystem::~FSG_LoggingSystem()
{
    Shutdown();
}

void FSG_LoggingSystem::Shutdown()
{
    if (bIsInitialized)
    {
        // Flush any pending logs
        if (LogFile)
        {
            LogFile->Flush();
        }
        
        // Close the log file
        if (LogFile)
        {
            LogFile->Close();
            delete LogFile;
            LogFile = nullptr;
        }
        
        bIsInitialized = false;
    }
}

void FSG_LoggingSystem::Log(const FString& Message, ESG_LogVerbosity Verbosity, 
                          const FString& Category, ESG_LogOutputTarget OutputTarget)
{
    if (!bIsInitialized) return;
    
    ActiveLogs.Increment();
    InternalLog(Message, Verbosity, Category, OutputTarget);
    ActiveLogs.Decrement();
}

void FSG_LoggingSystem::LogWithContext(const FString& Message, const UObject* ContextObject, 
                                     ESG_LogVerbosity Verbosity, ESG_LogOutputTarget OutputTarget)
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
                                  const FString& Context)
{
    // Check verbosity level
    {
        FScopeLock ConfigLock(&ConfigMutex);
        const ESG_LogVerbosity* CategoryVerbosityPtr = CategoryVerbosity.Find(Category);
        const ESG_LogVerbosity CurrentVerbosity = CategoryVerbosityPtr ? *CategoryVerbosityPtr : GlobalVerbosity;
        
        if (Verbosity > CurrentVerbosity)
        {
            return; // Skip if below verbosity threshold
        }
    }
    
    // Format the log message
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y.%m.%d-%H:%M:%S"));
    
    // Get enum as string
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
    
    // Output to targets
    if (EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::File) && LogFile)
    {
        FScopeLock FileLock(&FileMutex);
        LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
        
        // Check if we need to rotate the log file
        if (LogFile->Tell() >= MaxLogFileSize)
        {
            RotateLogFile();
        }
    }
    
    if (EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::Console))
    {
        // Convert our verbosity to ELogVerbosity
        ELogVerbosity::Type UnrealVerbosity = ELogVerbosity::Log;
        switch (Verbosity)
        {
            case ESG_LogVerbosity::Fatal: UnrealVerbosity = ELogVerbosity::Fatal; break;
            case ESG_LogVerbosity::Error: UnrealVerbosity = ELogVerbosity::Error; break;
            case ESG_LogVerbosity::Warning: UnrealVerbosity = ELogVerbosity::Warning; break;
            case ESG_LogVerbosity::Display: UnrealVerbosity = ELogVerbosity::Display; break;
            case ESG_LogVerbosity::Log: UnrealVerbosity = ELogVerbosity::Log; break;
            case ESG_LogVerbosity::Verbose: UnrealVerbosity = ELogVerbosity::Verbose; break;
            case ESG_LogVerbosity::VeryVerbose: UnrealVerbosity = ELogVerbosity::VeryVerbose; break;
        }
        
        FMsg::Logf(__FILE__, __LINE__, *Category, UnrealVerbosity, TEXT("%s"), *Message);
    }
    
    if (EnumHasAnyFlags(OutputTarget, ESG_LogOutputTarget::Screen) && GEngine)
    {
        FColor DisplayColor;
        float DisplayTime = 5.0f;
        
        switch (Verbosity)
        {
            case ESG_LogVerbosity::Fatal:
            case ESG_LogVerbosity::Error:
                DisplayColor = FColor::Red;
                DisplayTime = 10.0f;
                break;
            case ESG_LogVerbosity::Warning:
                DisplayColor = FColor::Yellow;
                DisplayTime = 7.0f;
                break;
            case ESG_LogVerbosity::Display:
                DisplayColor = FColor::Green;
                break;
            default:
                DisplayColor = FColor::White;
                break;
        }
        
        GEngine->AddOnScreenDebugMessage(-1, DisplayTime, DisplayColor, LogMessage);
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
void FSG_LoggingSystem::InitializeLogFile()
{
    if (LogFilePath.IsEmpty())
    {
        // Default log file path: {ProjectDir}/Saved/Logs/SG_Game.log
        LogFilePath = FPaths::ProjectLogDir() / TEXT("SG_Game.log");
    }
    
    // Create directory if it doesn't exist
    FString Directory = FPaths::GetPath(LogFilePath);
    IFileManager::Get().MakeDirectory(*Directory, true);
    
    // Open the log file
    LogFile = IFileManager::Get().CreateFileWriter(*LogFilePath, FILEWRITE_Append);
    
    if (LogFile)
    {
        // Write a header
        const FString Header = FString::Printf(
            TEXT("\n=== SG Log Started: %s ===\n\n"), 
            *FDateTime::Now().ToString(TEXT("%Y.%m.%d-%H:%M:%S")));
        
        LogFile->Serialize(TCHAR_TO_ANSI(*Header), Header.Len());
        LogFile->Flush();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create log file: %s"), *LogFilePath);
    }
}

void FSG_LoggingSystem::RotateLogFile()
{
    if (!LogFile) return;
    
    // Close current log file
    LogFile->Close();
    delete LogFile;
    LogFile = nullptr;
    
    // Generate timestamp for the backup
    const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    const FString BackupPath = FPaths::GetPath(LogFilePath) / 
                              FPaths::GetBaseFilename(LogFilePath) + 
                              TEXT("_") + Timestamp + 
                              TEXT(".log");
    
    // Rename current log to backup
    IFileManager::Get().Move(*BackupPath, *LogFilePath);
    
    // Reinitialize log file
    InitializeLogFile();
    
    // Clean up old log files if we have too many
    if (MaxBackupFiles > 0)
    {
        TArray<FString> LogFiles;
        IFileManager::Get().FindFiles(LogFiles, *FPaths::Combine(
            FPaths::GetPath(LogFilePath), 
            FPaths::GetBaseFilename(LogFilePath) + TEXT("_*") + 
            FPaths::GetExtension(LogFilePath, true)), true, false);
            
        LogFiles.Sort();
        
        // Delete oldest files if we have more than MaxBackupFiles
        for (int32 i = 0; i < LogFiles.Num() - MaxBackupFiles; ++i)
        {
            IFileManager::Get().Delete(*(FPaths::GetPath(LogFilePath) / LogFiles[i]));
        }
    }
}

// Configuration functions
void FSG_LoggingSystem::SetLogFile(const FString& FilePath)
{
    FScopeLock Lock(&FileMutex);
    
    if (LogFile)
    {
        LogFile->Close();
        delete LogFile;
        LogFile = nullptr;
    }
    
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

// Global accessor implementation
ISG_LoggingSystem& GetSGLoggingSystem()
{
    if (!GLoggingSystem.IsValid())
    {
        GLoggingSystem = MakeUnique<FSG_LoggingSystem>();
    }
    return *GLoggingSystem;
}
