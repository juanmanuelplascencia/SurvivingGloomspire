// Copyright (c) 2025 Surviving Gloomspire Development Team
// 
// This file contains original content and is subject to the MIT License.
// 
// The MIT License can be found in LICENSE.md

# Development Guidelines

This document outlines the development practices and standards for the Surviving Gloomspire project.

## Table of Contents
1. [Code Style](#code-style)
2. [Version Control](#_version_control)
3. [Testing](#testing)
4. [Documentation](#documentation)
5. [Performance](#performance)
6. [Security](#security)
7. [Build Process](#build-process)

## Code Style

### General
- Follow the [Unreal Engine Coding Standard](https://docs.unrealengine.com/5.0/en-US/epic-cplusplus-coding-standard-for-unreal-engine/)
- Use PascalCase for class names and camelCase for variable names
- Keep lines under 120 characters
- Use meaningful names for variables, functions, and classes
- Add comments for non-obvious code
- Use `#pragma once` in header files

### C++ Specific
- Use `USTRUCT()`, `UCLASS()`, `UFUNCTION()`, and `UPROPERTY()` macros appropriately
- Use `TArray`, `TMap`, and other UE container classes
- Prefer `TSharedPtr` and `TWeakPtr` over raw pointers
- Use `const` correctness
- Use `ensure()` for runtime checks
- Use `check()` for critical assertions

### Blueprint Specific
- Organize Blueprints in appropriate folders
- Use consistent naming conventions (e.g., BP_ for Blueprint assets)
- Keep Blueprints clean and well-commented
- Use Blueprint function libraries for reusable functions

## Version Control

### Commit Messages
Follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:
- `feat:` A new feature
- `fix:` A bug fix
- `docs:` Documentation only changes
- `style:` Changes that do not affect the meaning of the code
- `refactor:` A code change that neither fixes a bug nor adds a feature
- `perf:` A code change that improves performance
- `test:` Adding missing tests or correcting existing tests
- `chore:` Changes to the build process or auxiliary tools

Example:
```
feat(combat): add critical hit system

- Add critical hit calculation
- Add critical hit visual effects
- Update documentation

Fixes #123
```

## Testing

### Unit Tests
- Write unit tests for all new features
- Keep tests in the `Tests` directory
- Use the [Google Test](https://github.com/google/googletest) framework
- Test both success and failure cases
- Mock external dependencies

### Integration Tests
- Test interactions between systems
- Verify data flow between components
- Test save/load functionality

### Performance Testing
- Profile performance-critical code
- Monitor memory usage
- Test on minimum and recommended hardware

## Documentation

### Code Documentation
- Document all public interfaces
- Use Doxygen-style comments for C++
- Document Blueprint nodes and variables
- Keep documentation up-to-date

### System Documentation
- Update system documentation when making changes
- Add diagrams for complex systems
- Document design decisions

## Performance

### General Guidelines
- Profile before optimizing
- Optimize the critical path first
- Use UE's profiling tools (Unreal Insights, Profiler)
- Minimize memory allocations in tight loops
- Use object pooling where appropriate

### Memory Management
- Use UE's memory management system
- Avoid memory leaks
- Use appropriate container types
- Be mindful of memory fragmentation

### Network Optimization
- Minimize network traffic
- Use replication efficiently
- Compress network data when possible
- Implement client-side prediction where appropriate

## Security

### Input Validation
- Validate all user input
- Sanitize network data
- Use UE's security features

### Data Protection
- Encrypt sensitive data
- Validate save files
- Implement anti-cheat measures

## Build Process

### Building the Project
- Use the included build scripts
- Follow the build instructions in the main README
- Document any build requirements

### Continuous Integration
- All pushes trigger CI builds
- Tests must pass before merging
- Code coverage requirements must be met
- Static analysis is performed on all commits

## Code Review Process

1. Create a pull request
2. Request reviews from relevant team members
3. Address all review comments
4. Ensure all tests pass
5. Get required approvals
6. Squash and merge

## Dependencies

- Unreal Engine 5.0+
- Visual Studio 2022
- Windows 10/11
- [List any other dependencies]
