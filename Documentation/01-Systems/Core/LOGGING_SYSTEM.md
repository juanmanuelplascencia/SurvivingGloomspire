# Logging System

## Overview
The Logging System (`ILoggingSystem`) provides structured logging for debugging, monitoring, error tracking, and performance analysis across all systems and platforms.

## Core Features
- Structured logging
- Configurable verbosity levels
- Debug, Info, Warning, Error, Critical
- Log rotation and archiving
- Performance monitoring
- Error tracking and reporting
- Distributed tracing support
- Log compression
- Log validation
- Log security
- Log recovery
- Log optimization
- Log monitoring
- Log debugging

## Implementation Details

### Log Message Structure
```typescript
interface ILogMessage {
    timestamp: Date;
    level: LogLevel;
    message: string;
    category: string;
    source: string;
    metadata: any;
    context: any;
    validation: LogValidation;
    security: LogSecurity;
    recovery: LogRecovery;
    performance: LogPerformance;
}
```

### Log Categories
- Game
- System
- Network
- Performance
- Security
- UI
- Audio
- Physics
- Rendering
- Platform-specific

### Log Levels
- Debug
- Info
- Warning
- Error
- Critical
- Security
- Performance
- Validation
- Recovery

## Performance Considerations
- Async logging
- Batch processing
- Memory efficient
- Thread-safe
- Minimal overhead
- Log compression
- Log optimization
- Log monitoring
- Log recovery
- Log validation
- Log security
- Log debugging

## Error Handling
- Log failure recovery
- Error tracking
- Crash reporting
- Stack trace capture
- Error correlation
- Log validation errors
- Log security errors
- Log recovery errors
- Log monitoring errors
- Log debugging errors
- Log performance errors

## Security Features
- Log encryption
- Log validation
- Log security checks
- Log recovery
- Log monitoring
- Log debugging
- Log access control
- Log security logging
- Log security validation
- Log security recovery

## Best Practices
1. Use appropriate log levels
2. Include context in logs
3. Implement log rotation
4. Monitor log size
5. Use structured logging
6. Implement error correlation
7. Handle log failures
8. Use async logging
9. Monitor performance
10. Implement security logging
11. Use log compression
12. Implement log validation
13. Handle log recovery
14. Monitor log performance
15. Implement log debugging
16. Use log optimization
17. Handle log security
18. Implement log monitoring
19. Use log recovery
20. Handle log validation

## Platform-Specific Considerations
- iOS logging
- Android logging
- Windows logging
- Cloud logging
- Cross-platform logging
- Platform-specific optimization
- Platform-specific security
- Platform-specific recovery

## Future Improvements
1. Enhanced log compression
2. Improved log validation
3. Better log recovery
4. Enhanced log security
5. Improved log performance
6. Better platform support
7. Enhanced log compatibility
8. Improved log reliability
9. Better log optimization
10. Enhanced log user experience

## Troubleshooting
1. Log delivery failures
2. Log processing errors
3. Platform-specific issues
4. Network sync issues
5. Log performance issues
6. Security validation issues
7. Log recovery issues
8. Log compatibility issues

## Performance Metrics
- Log processing time
- Log delivery time
- Log validation time
- Log recovery time
- Log compression ratio
- Log encryption time
- Log validation success rate
- Log error rate
- Log conflict rate

## Security Validation
- Log integrity
- Log encryption strength
- Log validation success
- Log recovery success
- Log security compliance
- Log access control
- Log encryption validation
- Log validation security
- Log recovery security
- Log security compliance

## User Experience
- Log success feedback
- Log progress indication
- Log error handling
- Log recovery options
- Log validation feedback
- Log performance feedback
- Log security feedback
- Log compatibility feedback
- Log user guidance
- Log troubleshooting guidance with key-value pairs
- Performance metrics
- Log rotation and archiving

## Interface

```cpp
/**
 * Log levels for message filtering
 */
UENUM(BlueprintType)
enum class ELogLevel : uint8
{
    Trace   UMETA(DisplayName = "Trace"),
    Debug   UMETA(DisplayName = "Debug"),
    Info    UMETA(DisplayName = "Info"),
    Warning UMETA(DisplayName = "Warning"),
    Error   UMETA(DisplayName = "Error"),
    Fatal   UMETA(DisplayName = "Fatal")
};

/**
 * Interface for log output targets
 */
class ILogOutput
{
public:
    virtual ~ILogOutput() = default;
    virtual void WriteLog(ELogLevel Level, const FString& Category, const FString& Message, 
                         const TMap<FString, FString>& Context) = 0;
};

/**
 * Main logging system interface
 */
class ILoggingSystem
{
public:
    virtual ~ILoggingSystem() = default;

    // Core Logging Methods
    virtual void Log(ELogLevel Level, const FString& Category, const FString& Message) = 0;
    virtual void LogWithContext(ELogLevel Level, const FString& Category, 
                              const FString& Message, const TMap<FString, FString>& Context) = 0;
    
    // Convenience Methods
    virtual void Fatal(const FString& Category, const FString& Message) = 0;
    virtual void Error(const FString& Category, const FString& Message) = 0;
    virtual void Warning(const FString& Category, const FString& Message) = 0;
    virtual void Info(const FString& Category, const FString& Message) = 0;
    virtual void Debug(const FString& Category, const FString& Message) = 0;
    virtual void Trace(const FString& Category, const FString& Message) = 0;

    // Configuration
    virtual void SetLogLevel(ELogLevel Level) = 0;
    virtual void SetCategoryLevel(const FString& Category, ELogLevel Level) = 0;
    virtual ELogLevel GetCategoryLevel(const FString& Category) const = 0;
    
    // Output Control
    virtual void AddOutputTarget(TSharedPtr<ILogOutput> Output) = 0;
    virtual void RemoveOutputTarget(TSharedPtr<ILogOutput> Output) = 0;
    
    // Metrics
    virtual void LogMetric(const FString& Name, float Value, const TMap<FString, FString>& Tags = {}) = 0;
    virtual void IncrementCounter(const FString& Name, const TMap<FString, FString>& Tags = {}) = 0;
};
```

## Implementation Details

### Core Components

1. **Log Manager**
   - Singleton instance
   - Manages log outputs and categories
   - Handles log message routing

2. **Log Queue**
   - Thread-safe queue for log messages
   - Processes logs on a background thread
   - Prevents logging from blocking game thread

3. **Output Targets**
   - Console output
   - File output with rotation
   - In-game console
   - Network logging (optional)

### Performance Considerations
- Uses lock-free queue for thread safety
- Minimal memory allocations in hot paths
- Async processing of log messages
- Configurable buffer sizes

## Usage Examples

### Basic Logging
```cpp
// Get the logging system
auto& Logger = GetLoggingSystem();

// Simple log
Logger.Info("Combat", "Player attacked enemy");

// With context
TMap<FString, FString> Context;
Context.Add("Damage", FString::FromInt(DamageAmount));
Context.Add("Target", Target->GetName());
Logger.LogWithContext(ELogLevel::Info, "Combat", "Attack resolved", Context);
```

### Configuration
```cpp
// Set minimum log level
Logger.SetLogLevel(ELogLevel::Debug);

// Set specific category level
Logger.SetCategoryLevel("Network", ELogLevel::Warning);
Logger.SetCategoryLevel("AI", ELogLevel::Debug);

// Add file output
auto FileOutput = MakeShared<FFileLogOutput>(TEXT("Game.log"));
Logger.AddOutputTarget(FileOutput);
```

### Metrics
```cpp
// Log performance metric
Logger.LogMetric("FrameTime", 16.67f, {{ "Level", "Dungeon1" }});

// Increment counter
Logger.IncrementCounter("EnemiesKilled", {{ "EnemyType", "Goblin" }});
```

## Log Categories

| Category        | Description                          | Default Level |
|-----------------|--------------------------------------|---------------|
| Game           | General game flow                    | Info          |
| Combat         | Combat-related events                | Info          |
| Network        | Network communication                | Warning       |
| Save           | Save/load operations                 | Info          |
| AI             | AI decision making                   | Debug         |
| Inventory      | Item management                      | Info          |
| UI             | User interface events                | Warning       |
| Performance    | Performance metrics                  | Info          |
| Input          | Player input handling                | Debug         |
| Physics        | Physics system                       | Warning       |
| Animation      | Animation system                     | Debug         |
| Audio          | Audio system                         | Info          |
| Script         | Gameplay scripting                   | Warning       |
| Memory         | Memory allocation/deallocation       | Warning       |
| Asset          | Asset loading/unloading              | Info          |


## Best Practices

1. **Use Appropriate Log Levels**
   - Fatal: Only for unrecoverable errors
   - Error: For handled exceptions and errors
   - Warning: For unexpected but recoverable conditions
   - Info: For important application flow
   - Debug: For development debugging
   - Trace: For very verbose tracing

2. **Structured Logging**
   - Use key-value pairs for structured data
   - Include relevant context with each log
   - Avoid string concatenation in log messages

3. **Performance Considerations**
   - Use category filtering to reduce log volume
   - Be mindful of log calls in performance-critical code
   - Consider using compile-time log level filtering

## Configuration File

The logging system can be configured via `DefaultLogging.ini`:

```ini
[/Script/LoggingSystem.LoggingSettings]
DefaultLogLevel=Info

[Categories]
Game=Info
Combat=Debug
Network=Warning
AI=Debug
Save=Info
UI=Warning
Performance=Info
Input=Debug
Physics=Warning
Animation=Debug
Audio=Info
Script=Warning
Memory=Warning
Asset=Info

[FileOutput]
Enabled=True
Filename=Logs/Game.log
MaxFileSizeMB=10
MaxBackupFiles=5

[ConsoleOutput]
Enabled=True

[NetworkOutput]
Enabled=False
ServerAddress=127.0.0.1
Port=12345
```

## Integration with Other Systems

### With Event System
```cpp
// Subscribe to important game events
EventSystem->Subscribe("Combat.DamageDealt", [&](const FEventData& Event) {
    LoggingSystem->Info("Combat", 
                       FString::Printf(TEXT("Dealt %d damage"), Event.GetInt("Amount")),
                       Event.GetContext());
});
```

### With Save System
```cpp
void USaveSystem::SaveGame()
{
    LoggingSystem->Info("Save", "Beginning save process");
    // Save logic...
    LoggingSystem->Info("Save", "Save completed successfully");
}
```

## Testing

### Unit Tests
```cpp
TEST_METHOD(TestLoggingSystem_BasicLogging)
{
    auto Logger = MakeUnique<FLoggingSystem>();
    auto TestOutput = MakeShared<FTestLogOutput>();
    Logger->AddOutputTarget(TestOutput);
    
    Logger->Info("Test", "Test message");
    
    // Verify log was received with correct level and message
    Assert::IsTrue(TestOutput->HasMessage(ELogLevel::Info, "Test", "Test message"));
}
```

### Performance Tests
- Measure throughput under load
- Test thread safety with concurrent loggers
- Profile memory usage with high log volume

## Future Enhancements

1. **Log Aggregation**
   - Centralized log collection
   - Real-time log analysis
   - Alerting on error conditions

2. **Advanced Filtering**
   - Regular expression filters
   - Time-based filtering
   - Category hierarchies

3. **Visualization**
   - Performance graphs
   - Error heatmaps
   - Interactive log viewer

## Dependencies
- Core module
- Platform-specific file I/O
- Threading utilities
- Configuration system

## See Also
- [Event System Documentation](./SYSTEM_EVENTS.md)

---
*Last Updated: 2025-06-10*
