// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "ISG_LoggingSystem.h"

// Base logging macro
#define SG_LOG(Verbosity, Category, Format, ...) \
    GetSGLoggingSystem().Log(FString::Printf(TEXT(Format), ##__VA_ARGS__), \
                            ESG_LogVerbosity::Verbosity, \
                            TEXT(Category), \
                            ESG_LogOutputTarget::All)

// Context-aware logging
#define SG_LOG_CTX(Verbosity, Category, ContextObject, Format, ...) \
    GetSGLoggingSystem().LogWithContext(FString::Printf(TEXT(Format), ##__VA_ARGS__), \
                                       ContextObject, \
                                       ESG_LogVerbosity::Verbosity, \
                                       ESG_LogOutputTarget::All)

// Performance timing
#define SG_START_TIMER(TimerName) \
    GetSGLoggingSystem().StartTimer(TEXT(TimerName))

#define SG_STOP_TIMER(TimerName) \
    GetSGLoggingSystem().StopTimer(TEXT(TimerName))

#define SG_STOP_TIMER_MSG(TimerName, Message) \
    GetSGLoggingSystem().StopTimer(TEXT(TimerName), TEXT(Message))

// Memory tracking
#define SG_TRACK_MEMORY(Tag) \
    GetSGLoggingSystem().TrackMemory(TEXT(Tag))

#define SG_UNTRACK_MEMORY(Tag) \
    GetSGLoggingSystem().UntrackMemory(TEXT(Tag))

// Verbosity-specific macros
#define SG_LOG_FATAL(Category, Format, ...) \
    SG_LOG(Fatal, Category, Format, ##__VA_ARGS__)

#define SG_LOG_ERROR(Category, Format, ...) \
    SG_LOG(Error, Category, Format, ##__VA_ARGS__)

#define SG_LOG_WARNING(Category, Format, ...) \
    SG_LOG(Warning, Category, Format, ##__VA_ARGS__)

#define SG_LOG_DISPLAY(Category, Format, ...) \
    SG_LOG(Display, Category, Format, ##__VA_ARGS__)

#define SG_LOG_VERBOSE(Category, Format, ...) \
    SG_LOG(Verbose, Category, Format, ##__VA_ARGS__)

#define SG_LOG_VERY_VERBOSE(Category, Format, ...) \
    SG_LOG(VeryVerbose, Category, Format, ##__VA_ARGS__)

// Context-aware logging with auto-category
#define SG_LOG_CTX_FATAL(ContextObject, Format, ...) \
    SG_LOG_CTX(Fatal, "Game", ContextObject, Format, ##__VA_ARGS__)

#define SG_LOG_CTX_ERROR(ContextObject, Format, ...) \
    SG_LOG_CTX(Error, "Game", ContextObject, Format, ##__VA_ARGS__)

#define SG_LOG_CTX_WARNING(ContextObject, Format, ...) \
    SG_LOG_CTX(Warning, "Game", ContextObject, Format, ##__VA_ARGS__)

// Scoped timer for RAII-style timing
class SG_SCOPED_TIMER
{
public:
    SG_SCOPED_TIMER(const FString& InName, const FString& InMessage = TEXT(""))
        : Name(InName)
        , Message(InMessage)
    {
        GetSGLoggingSystem().StartTimer(Name);
    }
    
    ~SG_SCOPED_TIMER()
    {
        GetSGLoggingSystem().StopTimer(Name, Message);
    }
    
private:
    FString Name;
    FString Message;
};

// Memory tracking scope
class SG_SCOPED_MEMORY_TRACKER
{
public:
    SG_SCOPED_MEMORY_TRACKER(const FString& InTag)
        : Tag(InTag)
    {
        GetSGLoggingSystem().TrackMemory(Tag);
    }
    
    ~SG_SCOPED_MEMORY_TRACKER()
    {
        GetSGLoggingSystem().UntrackMemory(Tag);
    }
    
private:
    FString Tag;
};
