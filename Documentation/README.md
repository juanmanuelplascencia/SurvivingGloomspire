# Surviving Gloomspire Documentation

Welcome to the documentation repository for Surviving Gloomspire, a tactical survival game built with Unreal Engine 5.

## Documentation Structure

### 1. [Core Documentation](00-Core/)
- [Game Vision](00-Core/GameVision.md) - Core game concept and vision
- [System Architecture](00-Core/ARCHITECTURE.md) - Comprehensive system architecture overview
- [Technical Specifications](00-Core/TECHNICAL_SPECS.md) - Technical requirements and specifications
- [Development Roadmap](00-Core/ROADMAP_PHASE_0.md) - Current development phase roadmap

### 2. [Core Systems](01-Systems/Core/)
- [Event System (IEventSystem)](01-Systems/Core/EVENT_SYSTEM.md) - Event-driven architecture
- [Effect System (IEffectSystem)](01-Systems/Core/EFFECT_SYSTEM.md) - Game effects management
- [Dice System (IDiceSystem)](01-Systems/Core/DICE_SYSTEM.md) - Dice mechanics implementation
- [Rules Engine (IRulesEngine)](01-Systems/Core/RULES_ENGINE.md) - Game rules processing
- [Save System (ISaveSystem)](01-Systems/Core/SAVE_SYSTEM.md) - Game state persistence
- [Logging System (ILoggingSystem)](01-Systems/Core/LOGGING_SYSTEM.md) - Logging and debugging

### 3. [Development Guidelines](02-Development/)
- [Development Practices](02-Development/DEVELOPMENT.md) - Development workflow and standards
- [Coding Standards](02-Development/CODING_STANDARDS.md) - Code style and best practices
- [Performance Guidelines](02-Development/PERFORMANCE_GUIDELINES.md) - Optimization strategies
- [Testing Strategy](02-Development/TESTING_STRATEGY.md) - Testing approach and procedures

### 4. [Project Management](03-Process/)
- [Phase 0 Checklist](03-Process/PHASE_0_CHECKLIST.md) - Detailed task breakdown
- [Risk Register](03-Process/RISK_REGISTER.md) - Risk assessment and mitigation

### 5. [API Documentation](04-API/)
*(Coming soon - API reference documentation will be generated from code)*

### 6. [Asset Guidelines](05-Assets/)
*(Coming soon - Art and audio asset guidelines)*

## License Information

This project uses two licenses:

1. **Original Code (MIT License)**
   - All original code and assets created for this project
   - See [LICENSE.md](../LICENSE.md) for full MIT License text

2. **Pathfinder Content (Open Game License v1.0)**
   - All Pathfinder rules, mechanics, and content
   - See http://paizo.com/pathfinderRPG/ogl for full OGL text

## Getting Started

### For New Team Members
1. Start with the [Game Vision](00-Core/GameVision.md) to understand the core concept
2. Review the [System Architecture](00-Core/ARCHITECTURE.md) for high-level design
3. Check the [Development Roadmap](00-Core/ROADMAP_PHASE_0.md) for current status
4. Follow the [Coding Standards](02-Development/CODING_STANDARDS.md)

### For Developers
1. Set up your environment using [Development Practices](02-Development/DEVELOPMENT.md)
2. Follow the [Testing Strategy](02-Development/TESTING_STRATEGY.md)
3. Adhere to [Performance Guidelines](02-Development/PERFORMANCE_GUIDELINES.md)

### For Contributors
1. Review the [Phase 0 Checklist](03-Process/PHASE_0_CHECKLIST.md)
2. Check the [Risk Register](03-Process/RISK_REGISTER.md)
3. Follow the contribution guidelines below

## Contributing

We welcome contributions to Surviving Gloomspire! Please follow these guidelines:

1. **Code Contributions**
   - Follow our [CODING_STANDARDS.md](CODING_STANDARDS.md)
   - Write tests for new features
   - Document your code and update relevant documentation
   - Keep commits focused and atomic

2. **Documentation**
   - Keep documentation in sync with code changes
   - Follow the existing documentation style
   - Add new documentation for new features
   - Update the [RISK_REGISTER.md](RISK_REGISTER.md) if you identify new risks

3. **Pull Requests**
   - Reference related issues
   - Include a clear description of changes
   - Ensure all tests pass
   - Get necessary approvals before merging

4. **Reporting Issues**
   - Use the issue templates
   - Include reproduction steps
   - Specify affected versions
   - Add relevant logs or screenshots

## License

[Your License Information Here]
