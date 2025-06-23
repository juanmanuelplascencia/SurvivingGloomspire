# Surviving Gloomspire

A Pathfinder 1st Edition implementation in Unreal Engine 5.

## Project Setup

### Prerequisites
- Unreal Engine 5.6+
- Visual Studio 2022 with C++ workload
- Git LFS (for version control)

### Getting Started
1. Clone the repository
2. Run `git lfs install` if you haven't already
3. Open `SurvivingGloomspire.uproject`
4. Let the engine build the project

## Project Structure

### Content/
- `Blueprints/`: Gameplay blueprints
- `Characters/`: Character assets
- `Core/`: Core game systems
  - `Data/`: Data assets
  - `GameModes/`: Game mode implementations
  - `UI/`: Core UI elements
- `Data/`: Game data
- `Maps/`: Game levels
- `Materials/`: Material assets
- `UI/`: User interface assets
- `VFX/`: Visual effects

### Source/
- `SurvivingGloomspire/`: C++ source code
  - `Core/`: Core game systems
    - `Components/`: Actor components
    - `Data/`: Data structures
    - `Gameplay/`: Gameplay systems
  - `Game/`: Game-specific implementations
    - `Characters/`: Character classes
    - `GameModes/`: Game mode implementations
  - `Public/`: Public headers

## Development

### Coding Standards
- Follow Unreal Engine's coding standards
- Use PascalCase for C++ classes and structs
- Use camelCase for variables and functions
- Prefix boolean variables with b (e.g., bIsActive)

### Version Control
- Use feature branches for new features
- Create pull requests for code review
- Write meaningful commit messages

## License
[Your License Here]
