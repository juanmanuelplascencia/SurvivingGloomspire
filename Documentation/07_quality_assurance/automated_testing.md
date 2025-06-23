# Automated Testing Framework

## Overview
This document details the automated testing infrastructure for Surviving Gloomspire, ensuring consistent quality and rapid feedback during development.

## Test Categories

### 1. Unit Tests
- **Location**: `Source/SurvivingGloomspire/Tests/Unit`
- **Coverage**:
  - Game mechanics
  - Data structures
  - Utility functions
- **Example**:
  ```cpp
  IMPLEMENT_SIMPLE_AUTOMATION_TEST(
      FCombatDamageTest,
      "Game.Combat.DamageCalculation",
      EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter
  )
  
  bool FCombatDamageTest::RunTest(const FString& Parameters)
  {
      // Test damage calculation
      float Damage = UCombatLibrary::CalculateDamage(100.0f, 0.5f, 1.2f);
      TestEqual(TEXT("Damage calculation"), Damage, 60.0f);
      return true;
  }
  ```

### 2. Functional Tests
- **Location**: `Source/SurvivingGloomspire/Tests/Functional`
- **Coverage**:
  - Game features
  - Player controls
  - AI behavior
- **Example**:
  ```python
  def test_character_movement():
      # Spawn character
      character = spawn_test_character()
      start_pos = character.get_actor_location()
      
      # Move character
      character.move_forward(1.0)  # Full input for 1 second
      
      # Verify movement
      end_pos = character.get_actor_location()
      assert vector_distance(start_pos, end_pos) > 0, "Character did not move"
  ```

### 3. Performance Tests
- **Location**: `Source/SurvivingGloomspire/Tests/Performance`
- **Metrics**:
  - Frame time
  - Memory usage
  - Load times
- **Thresholds**:
  ```ini
  [PerformanceThresholds]
  FrameTime=16.67ms  # 60 FPS
  MemoryLeak=5MB
  LoadTime=30s
  ```

## Test Automation Framework

### 1. Test Runner
- **Engine**: Unreal Automation Framework
- **Languages**: C++, Python
- **Features**:
  - Parallel test execution
  - Test filtering
  - HTML/JSON reporting

### 2. CI/CD Integration
```yaml
# .github/workflows/tests.yml
name: Run Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v3
    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.9'
    - name: Run Unit Tests
      run: "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="%GITHUB_WORKSPACE%\SurvivingGloomspire.uproject" -noP4 -clientconfig=Test -serverconfig=Test -nocompile -nocompileeditor -installed -ue4exe=UE4Editor-Cmd.exe -test="Unit" -log=Tests/Logs/UnitTests.log"
    - name: Run Functional Tests
      if: always()
      run: "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="%GITHUB_WORKSPACE%\SurvivingGloomspire.uproject" -noP4 -clientconfig=Test -serverconfig=Test -nocompile -nocompileeditor -installed -ue4exe=UE4Editor-Cmd.exe -test="Functional" -log=Tests/Logs/FunctionalTests.log"
    - name: Upload Test Results
      uses: actions/upload-artifact@v3
      if: always()
      with:
        name: test-results
        path: Tests/Logs/
```

### 3. Test Data Management
- **Fixtures**: Pre-configured test levels
- **Mocks**: Simulated services
- **Factories**: Test object creation

## Test Coverage

### 1. Code Coverage
- **Target**: 80%+ for core systems
- **Tools**:
  - Unreal's Code Coverage plugin
  - OpenCppCoverage
  - ReportGenerator

### 2. Test Reporting
- **Formats**:
  - JUnit XML
  - HTML
  - Slack/Email notifications
- **Metrics**:
  - Pass/fail rates
  - Test duration
  - Flaky tests

## Performance Testing

### 1. Automated Benchmarks
```python
def test_combat_performance():
    # Setup combat scenario
    characters = spawn_characters(count=10)
    start_time = time.time()
    
    # Run combat simulation
    for _ in range(1000):
        simulate_combat_round(characters)
    
    # Verify performance
    duration = time.time() - start_time
    assert duration < 1.0, "Combat simulation too slow"
```

### 2. Memory Profiling
- **Tools**:
  - Unreal Memory Profiler
  - VMMap
  - Custom allocation tracking

## Best Practices

### 1. Test Design
- **AAA Pattern**: Arrange, Act, Assert
- **FIRST Principles**:
  - Fast
  - Independent
  - Repeatable
  - Self-validating
  - Timely

### 2. Maintenance
- **Review**: Weekly test reviews
- **Update**: With feature changes
- **Cleanup**: Remove obsolete tests

## Local Development

### 1. Running Tests
```bash
# Run all tests
./RunTests.sh

# Run specific test category
./RunTests.sh -test=Unit

# Run single test
./RunTests.sh -test=Game.Combat.DamageCalculation
```

### 2. Debugging
- **Flags**: `-log -stdout -fullstdlogoutput`
- **Logs**: `Saved/Logs/Test.log`
- **Visual Debugger**: Unreal Frontend

## Test Environment

### 1. Requirements
- **Hardware**:
  - CPU: Quad-core 2.5 GHz+
  - RAM: 16GB+
  - GPU: DirectX 11/12 compatible

### 2. Dependencies
- **Engine Plugins**:
  - Automation Controller
  - Functional Testing
  - Editor Scripting Utilities

## Monitoring and Alerting

### 1. Test Metrics
- **Dashboard**: Grafana
- **Storage**: InfluxDB
- **Alerting**: PagerDuty

### 2. Flaky Test Management
- **Detection**: Historical analysis
- **Quarantine**: Auto-disable flaky tests
- **Tracking**: JIRA integration

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
