# Code Style Guide

## General Principles

### Readability First
- Code should be self-documenting
- Favor clarity over cleverness
- Follow the principle of least surprise

### Consistency
- Follow established patterns
- Match the style of existing code
- When in doubt, follow the Unreal Engine coding standard

## Naming Conventions

### Files
- Header files: `PascalCase.h`
- Source files: `PascalCase.cpp`
- Blueprints: `BP_DescriptiveName`
- Assets: `Prefix_Descriptor_Variant`

### Classes & Structs
- `PascalCase`
- Nouns or noun phrases
- Examples:
  ```cpp
  class APlayerCharacter;
  struct FPlayerStats;
  ```

### Variables
- `camelCase`
- Descriptive names
- Examples:
  ```cpp
  float health;
  int32 score;
  FString playerName;
  ```

### Functions
- `PascalCase`
- Verb-noun pairs
- Examples:
  ```cpp
  void CalculateDamage();
  bool CanAttack() const;
  ```

### Boolean
- Prefix with `b`
- Questions or assertions
- Examples:
  ```cpp
  bool bIsAlive;
  bool bHasWeapon;
  ```

## Code Formatting

### Braces
- Allman style for functions and classes
- K&R style for control statements

```cpp
// Functions
void MyFunction()
{
    // ...
}

// Control flow
if (condition) {
    // ...
} else {
    // ...
}
```

### Indentation
- 4 spaces (no tabs)
- Align related items

### Line Length
- Maximum 120 characters
- Break long lines at logical points

## Unreal Engine Specific

### Class Prefixes
- `A` for Actors
- `U` for UObject-derived classes
- `F` for plain classes/structs
- `I` for interfaces
- `E` for enums
- `b` for booleans
- `In` for input parameters
- `Out` for output parameters

### Header Includes
1. Matching header
2. Engine includes
3. Project includes
4. Local includes

### Logging
- Use appropriate verbosity levels
- Include context in log messages
- Example:
  ```cpp
  UE_LOG(LogGame, Warning, TEXT("Player %s took %f damage"), 
      *PlayerName, DamageAmount);
  ```

## Commenting

### Documentation Comments
Use Doxygen-style for public APIs:

```cpp
/**
 * Calculates the damage to apply to the target.
 * @param DamageAmount Base damage amount
 * @param DamageType Type of damage being applied
 * @param Instigator Who caused the damage
 * @return Actual damage applied
 */
float CalculateDamage(float DamageAmount, const FDamageType& DamageType, 
                     AController* Instigator);
```

### Inline Comments
- Explain why, not what
- Use complete sentences
- Keep them up-to-date

## Performance Guidelines

### Memory Management
- Use `TArray`, `TMap`, `TSet` instead of STL
- Prefer `TArrayView` for read-only access
- Use `TUniquePtr` for unique ownership
- Use `TSharedPtr` for shared ownership

### Loops
- Use range-based for when possible
- Cache array lengths
- Example:
  ```cpp
  for (const auto& Item : Items) {
      // Process item
  }
  ```

## Testing

### Unit Tests
- One test file per class
- Test both success and failure cases
- Follow naming: `ClassBeingTested_TestScenario_ExpectedBehavior`

### Test Structure
```cpp
BEGIN_DEFINE_SPEC(FMyClassSpec, "Game.MyClass", EAutomationTestFlags::ProductFilter)
    TSharedPtr<FMyClass> TestInstance;
END_DEFINE_SPEC(FMyClassSpec)

void FMyClassSpec::Define()
{
    Describe("Functionality", [this]()
    {
        It("should work correctly", [this]()
        {
            // Test code
            TestTrue("Condition", true);
        });
    });
}
```

## Version Control

### Commit Messages
- Follow conventional commits:
  - `feat:` for new features
  - `fix:` for bug fixes
  - `docs:` for documentation
  - `refactor:` for code changes that don't fix bugs or add features
  - `test:` for adding tests
  - `chore:` for maintenance tasks

Example:
```
feat(Player): add double jump ability

- Implement double jump mechanic
- Add animation and sound effects
- Update player documentation

Resolves: #123
```

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
