# Project Setup Guide

## Prerequisites
- Unreal Engine 5.1 or later
- Visual Studio 2022 (for Windows)
- Git LFS (for version control)
- 8GB+ RAM (16GB recommended)
- DirectX 12 compatible GPU

## Initial Setup

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/surviving-gloomspire.git
cd surviving-gloomspire
git lfs install
git lfs pull
```

### 2. Generate Project Files
1. Right-click on `SurvivingGloomspire.uproject`
2. Select "Generate Visual Studio project files"
3. Wait for the process to complete

### 3. Open the Project
1. Double-click `SurvivingGloomspire.uproject`
2. Select your preferred IDE (Visual Studio recommended)
3. Wait for the project to load and shaders to compile

## Project Structure

```
SurvivingGloomspire/
├── Config/                  # Configuration files
├── Content/                 # Game assets
│   ├── Characters/          # Character models and animations
│   ├── Core/                # Core game assets
│   ├── Effects/             # Visual and sound effects
│   ├── Levels/              # Game levels
│   ├── UI/                  # User interface assets
│   └── ...
├── Documentation/          # Project documentation
├── Plugins/                 # Custom plugins
├── Source/                  # C++ source code
│   ├── SurvivingGloomspire/  # Game module
│   └── ...
├── .gitattributes          # Git LFS configuration
├── .gitignore              # Git ignore rules
└── SurvivingGloomspire.uproject  # Project file
```

## Development Workflow

### 1. Branching Strategy
- `main`: Stable, production-ready code
- `develop`: Integration branch for features
- `feature/`: Feature branches (e.g., `feature/combat-system`)
- `bugfix/`: Bug fix branches
- `release/`: Release preparation branches

### 2. Code Style
Follow the [Unreal Engine Coding Standard](https://docs.unrealengine.com/5.0/en-US/epic-cplusplus-coding-standard-for-unreal-engine/).

### 3. Commit Messages
Use the following format:
```
[Category] Brief description

- Detailed explanation of changes
- Additional notes
```

Categories:
- `[FEAT]` New features
- `[FIX]` Bug fixes
- `[DOC]` Documentation updates
- `[REFACTOR]` Code refactoring
- `[TEST]` Test additions/updates
- `[CHORE]` Maintenance tasks

## Building the Project

### Development Build
```bash
# Windows
Build.bat SurvivingGloomspireEditor Win64 Development -WaitMutex -FromMsBuild

# Linux
./Build.sh SurvivingGloomspireEditor Linux Development -WaitMutex -FromMsBuild
```

### Shipping Build
```bash
# Windows
Build.bat SurvivingGloomspire Win64 Shipping -WaitMutex -FromMsBuild

# Linux
./Build.sh SurvivingGloomspire Linux Shipping -WaitMutex -FromMsBuild
```

## Testing

### Running Tests
1. Open the project in the Unreal Editor
2. Go to Window > Developer Tools > Session Frontend
3. Select the "Automation" tab
4. Run the desired test suite

### Writing Tests
1. Create a new file in `Source/SurvivingGloomspire/Tests/`
2. Use the following template:

```cpp
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FMyTest,
    "Game.Tests.MyTest",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
)

bool FMyTest::RunTest(const FString& Parameters)
{
    // Test code here
    TestTrue("True should be true", true);
    return true;
}
```

## Troubleshooting

### Common Issues

#### 1. Missing Dependencies
```
Error: Missing modules
```
**Solution:**
1. Right-click the .uproject file
2. Select "Generate Visual Studio project files"
3. Rebuild the solution

#### 2. Shader Compilation Errors
```
Error: Failed to compile global shader
```
**Solution:**
1. Delete the `Saved/Shaders` folder
2. Right-click the .uproject file
3. Select "Generate Visual Studio project files"
4. Rebuild the solution

#### 3. Git LFS Issues
```
Error: Git LFS: smudge - filter lfs failed
```
**Solution:**
```bash
git lfs install
git lfs pull
git reset --hard
```

## Performance Guidelines

### Memory Management
- Use `UPROPERTY()` for UObject pointers
- Use `TWeakObjectPtr` for non-owning references
- Implement object pooling for frequently created/destroyed objects
- Use `TArray` with `Reserve()` when size is known in advance

### Performance Tips
- Profile regularly with `stat unit` and `stat unitgraph`
- Use the Unreal Insights tool for detailed profiling
- Optimize tick functions with `PrimaryActorTick.bCanEverTick = false` when possible
- Use `TArrayView` for read-only access to arrays
- Prefer `TMap` for large, frequently accessed data sets

## Additional Resources

### Documentation
- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [Unreal Engine API Reference](https://docs.unrealengine.com/API/)
- [Unreal Engine Community Wiki](https://unrealcommunity.wiki/)

### Tools
- [UnrealVS](https://docs.unrealengine.com/5.0/en-US/unreal-visual-studio-toolkit-vs-integration/)
- [Unreal Engine Marketplace](https://www.unrealengine.com/marketplace/)
- [Unreal Engine Forums](https://forums.unrealengine.com/)

### Community
- [Unreal Slackers Discord](https://unrealslackers.org/)
- [r/unrealengine](https://www.reddit.com/r/unrealengine/)
- [Unreal Engine AnswerHub](https://answers.unrealengine.com/)

## Support
For additional help, contact the project maintainers:
- Lead Developer: [Your Name] <your.email@example.com>
- Technical Director: [Name] <email@example.com>

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
