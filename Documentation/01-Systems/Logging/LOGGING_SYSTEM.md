# Logging System

The logging system provides a comprehensive solution for application logging, debugging, and crash reporting. It's designed to be thread-safe, flexible, and easy to use.

## Features

- **Multiple Log Levels**: Fatal, Error, Warning, Display, Log, Verbose, and VeryVerbose
- **Multiple Output Targets**: File, screen, and console output
- **Log Rotation**: Automatic log file rotation with configurable size limits
- **Performance Monitoring**: Built-in performance timers and counters
- **Memory Tracking**: Track memory usage by category
- **Crash Reporting**: Integrated crash reporting with detailed context (see [CRASH_REPORTING.md](CRASH_REPORTING.md))
- **Thread Safety**: Designed for safe use from multiple threads

## Core Components

### ISG_LoggingSystem

The main interface for the logging system. Provides methods for logging messages, managing verbosity, and configuring the logging system.

### FSG_LoggingSystem

The implementation of the logging system. Handles all logging operations and manages log files.

### Logging Macros

Convenience macros for common logging operations:

- `SG_LOG_FATAL`, `SG_LOG_ERROR`, `SG_LOG_WARNING`, etc.
- `SG_LOG_CTX` for context-aware logging
- `SG_START_TIMER`/`SG_STOP_TIMER` for performance measurement

## Usage

### Basic Logging

```cpp
// Get the logging system instance
ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();

// Log a message
LoggingSystem.Log("This is a log message", ESG_LogVerbosity::Log, "Category");

// Log with context (UObject)
LoggingSystem.LogWithContext("Player did something", PlayerPawn, ESG_LogVerbosity::Display);
```

### Performance Monitoring

```cpp
// Start a timer
LoggingSystem.StartTimer("ExpensiveOperation");

// ... expensive operation ...

// Stop the timer and log the duration
double Duration = LoggingSystem.StopTimer("ExpensiveOperation", "Finished expensive operation");
```

### Memory Tracking

```cpp
// Track memory usage
LoggingSystem.TrackMemory("TextureCache");

// ... allocate textures ...

// Stop tracking
LoggingSystem.UntrackMemory("TextureCache");
```

### Crash Reporting

See [CRASH_REPORTING.md](CRASH_REPORTING.md) for detailed information on crash reporting features.

## Configuration

```cpp
// Set global verbosity
LoggingSystem.SetGlobalVerbosity(ESG_LogVerbosity::Verbose);

// Set verbosity for a specific category
LoggingSystem.SetCategoryVerbosity("Network", ESG_LogVerbosity::VeryVerbose);

// Configure log file settings
LoggingSystem.SetMaxLogFileSize(10 * 1024 * 1024); // 10MB
LoggingSystem.SetMaxBackupFiles(5);
```

## Best Practices

1. **Use Appropriate Log Levels**: 
   - Fatal/Error: Critical issues that prevent normal operation
   - Warning: Issues that don't prevent operation but might cause problems
   - Display: Important information for the user
   - Log/Verbose: Debugging information
   - VeryVerbose: Extremely detailed debugging

2. **Use Categories**: Categorize logs to make filtering and searching easier.

3. **Be Mindful of Performance**: Avoid expensive operations in log messages, especially at high verbosity levels.

4. **Handle Sensitive Information**: Be careful not to log sensitive information like passwords or personal data.

5. **Test Logging**: Verify that your logs contain the information you need for debugging.

## Integration

The logging system is integrated with Unreal Engine's module system and is accessible via the `GetSGLoggingSystem()` function. It's automatically initialized when the game module starts up.
