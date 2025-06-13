# Crash Reporting System

The crash reporting system is integrated with the logging system to provide comprehensive crash reporting capabilities. It captures detailed information about application crashes and generates crash reports that can be used for debugging and analysis.

## Features

- Automatic crash detection and reporting
- Detailed crash context including callstack, system information, and memory stats
- Custom crash report generation
- Crash report callback system for custom handling
- Thread-safe implementation

## Usage

### Initialization

Crash reporting is automatically initialized when the logging system starts up. By default, crash reports are saved to `Logs/CrashReports/` in your project directory.

```cpp
// Get the logging system instance
ISG_LoggingSystem& LoggingSystem = GetSGLoggingSystem();

// Initialize crash reporting (called automatically during module startup)
LoggingSystem.InitializeCrashReporting();

// Set a custom crash report path (optional)
FString CrashReportPath = FPaths::ProjectLogDir() / TEXT("MyCustomCrashReports");
LoggingSystem.SetCrashReportPath(CrashReportPath);
```

### Custom Crash Callback

You can set a custom callback to handle crash reports:

```cpp
LoggingSystem.SetCrashReportCallback([](const SG_CRASH_CONTEXT& Context, FOutputDevice& Ar)
{
    // Custom crash report formatting
    Ar.Logf(TEXT("=== CUSTOM CRASH REPORT ===\n"));
    Ar.Logf(TEXT("Error: %s\n"), Context.ErrorMessage);
    
    if (Context.Function && *Context.Function)
    {
        Ar.Logf(TEXT("Function: %s\n"), ANSI_TO_TCHAR(Context.Function));
    }
    
    if (!Context.ContextInfo.IsEmpty())
    {
        Ar.Logf(TEXT("\nCallstack:\n%s\n"), *Context.ContextInfo);
    }
});
```

### Generating Test Crash Reports

You can generate test crash reports using the console command:

```
TestCrash report
```

Or trigger a test crash:

```
TestCrash crash
```

### Programmatic Crash Report Generation

Generate crash reports programmatically:

```cpp
LoggingSystem.GenerateCrashReport(
    TEXT("Something bad happened"),
    __FUNCTION__,
    __FILE__,
    __LINE__,
    0xDEADBEEF
);
```

## Crash Report Contents

Each crash report includes:

- Timestamp
- Error message
- Function, file, and line number (if available)
- Error code (if provided)
- Callstack
- System information (OS, CPU, memory)
- Custom context information

## Best Practices

1. **Set Up Early**: Initialize crash reporting as early as possible in your application startup.
2. **Custom Callbacks**: Use custom callbacks to add application-specific context to crash reports.
3. **Test Thoroughly**: Test crash reporting during development to ensure it works as expected.
4. **Handle Gracefully**: Ensure your application can handle crashes gracefully and save any necessary state.
5. **Monitor Disk Usage**: Be mindful of disk space when storing crash reports, especially in production.

## Troubleshooting

- **Missing Crash Reports**: Check the `Logs/CrashReports` directory in your project folder.
- **Incomplete Callstacks**: Ensure debug symbols are available for complete callstack information.
- **Permission Issues**: Make sure the application has write permissions to the crash report directory.

## Implementation Details

The crash reporting system uses platform-specific crash handling mechanisms and provides a unified interface for crash reporting across all supported platforms. It's designed to be as robust as possible, with minimal dependencies on other systems to ensure it works even when other parts of the application are failing.
