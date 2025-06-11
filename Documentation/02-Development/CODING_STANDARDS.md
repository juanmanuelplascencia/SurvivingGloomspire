# Coding Standards

## General Principles
- Clarity over cleverness
- Consistency across the codebase
- Self-documenting code
- Defensive programming
- KISS (Keep It Simple, Stupid)

## Naming Conventions

### Files
- Header files: `.h`
- Source files: `.cpp`
- Blueprint files: `BP_` prefix
- Interface classes: `I` prefix

### Classes and Structs
- PascalCase
- Nouns or noun phrases
- Clear and descriptive
- Avoid abbreviations

### Functions
- PascalCase
- Verb or verb phrase
- Clear action description
- Parameters in logical order

### Variables
- camelCase
- Descriptive names
- No Hungarian notation
- Use `b` prefix for booleans

### Constants
- ALL_CAPS
- Underscore separators
- Group related constants
- Document usage

## Code Organization

### Includes
- Group includes by category
- Alphabetical order within groups
- Use forward declarations when possible
- Remove unused includes

### Class Layout
1. Public section
2. Protected section
3. Private section
4. Friends
5. Types
6. Static members
7. Member variables
8. Constructors/Destructors
9. Methods

### Method Order
1. Public methods
2. Protected methods
3. Private methods
4. Callback methods
5. Event handlers
6. Helper methods

## Code Style

### Braces
- Allman style braces
- Always use braces, even for single statements
- One statement per line

### Indentation
- 4 spaces (no tabs)
- Case labels indented one level
- Access modifiers indented one level

### Line Length
- 120 character limit
- Break long lines at logical points
- Align wrapped lines appropriately

## Documentation

### File Headers
- Copyright notice
- Brief description
- Author information
- Change history

### Class Documentation
- Purpose of the class
- Key responsibilities
- Thread safety
- Usage examples
- Known limitations

### Function Documentation
- Purpose
- Parameters (in/out)
- Return value
- Exceptions thrown
- Pre/Post conditions

## Best Practices

### Memory Management
- Use smart pointers
- Follow RAII principles
- Check for nullptr
- Avoid raw pointers

### Error Handling
- Use exceptions for errors
- No silent failures
- Meaningful error messages
- Log errors appropriately

### Performance
- Avoid premature optimization
- Profile before optimizing
- Use const correctness
- Pass by const reference

## Unreal-Specific Guidelines

### Blueprints vs C++
- Gameplay logic in C++
- UI and level design in Blueprints
- Expose C++ to Blueprints when needed
- Keep Blueprint graphs clean

### UE Macros
- Use `UPROPERTY()` for properties
- Use `UFUNCTION()` for functions
- Use `GENERATED_BODY()`
- Follow Unreal's reflection system

### Logging
- Use `UE_LOG`
- Appropriate log levels
- Structured logging
- Performance-sensitive logging

## Code Review Guidelines
- Be constructive
- Explain reasoning
- Suggest alternatives
- Keep discussions professional
- Focus on the code, not the coder
