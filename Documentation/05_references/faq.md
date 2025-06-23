# Frequently Asked Questions

## General

### What is Surviving Gloomspire?
Surviving Gloomspire is a 2D game being developed with Unreal Engine 5, featuring strategic gameplay and deep progression systems set in the mysterious world of Gloomspire.

### What platforms will the game support?
The initial release will target PC (Windows), with potential for console and mobile platforms in the future.

### What is the development status?
We are currently in Phase 1 (Pre-production) of development. See our [Development Roadmap](02_development/01_roadmap/phase1_preproduction.md) for details.

## Technical

### What version of Unreal Engine are we using?
We're using Unreal Engine 5.6. Always ensure you have the correct version installed.

### How do I set up the development environment?
Follow the setup instructions in the [Technical Overview](01_project_overview/03_technical_overview.md).

### Where can I find the coding standards?
See the [Code Style Guide](05_references/01_style_guides/code_style.md) for detailed coding standards and best practices.

## Art & Assets

### What are the asset naming conventions?
Assets should follow the `Prefix_Descriptor_Variant` naming convention. See the [Asset Pipeline](02_development/05_asset_pipeline/) documentation for details.

### What are the texture requirements?
- Power of two dimensions (e.g., 256x256, 512x512, 1024x1024)
- PNG or TGA format
- sRGB enabled for color textures
- Normal maps should be compressed as BC5

## Design

### How do I document new game mechanics?
Create a new markdown file in the appropriate section under [Core Mechanics](02_development/03_core_mechanics/) following the [Documentation Style Guide](05_references/01_style_guides/documentation_style.md).

### Where can I find the game design documents?
All design documents are organized in the [Development](02_development/) section of the documentation.

## Version Control

### How do I submit changes?
1. Create a new branch for your feature/fix
2. Make your changes
3. Submit a pull request for review
4. Address any feedback
5. Merge when approved

### What should I include in commit messages?
Follow the [conventional commits](https://www.conventionalcommits.org/) format:
```
type(scope): short description

Longer description if needed

Resolves: #issue-number
```

## Testing

### How do I report a bug?
1. Check if the bug has already been reported
2. Create a new issue with a clear title and description
3. Include steps to reproduce
4. Add screenshots or videos if relevant
5. Label the issue appropriately

### What's the testing process?
1. Write unit tests for new features
2. Test locally
3. Submit for code review
4. Run automated tests in CI pipeline
5. Verify fixes in the test environment

## Performance

### What are the performance targets?
- 60 FPS on target hardware
- <2 second load times
- Memory usage under 2GB
- Build size under 4GB

### How can I optimize my code?
- Profile before optimizing
- Use Unreal's profiling tools
- Optimize only hotspots
- Consider data-oriented design
- Use appropriate data structures

## Legal

### Who owns the code I write?
All code and assets created as part of this project are owned by [Company Name].

### Can I use third-party assets?
Only with prior approval. All third-party assets must be properly licensed and documented.

## Getting Help

### Who should I contact if I have questions?
- Technical questions: [Lead Developer]
- Art/Design questions: [Lead Artist/Designer]
- General questions: [Project Manager]

### Where can I find more resources?
- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [Official Forums](https://forums.unrealengine.com/)
- [AnswerHub](https://answers.unrealengine.com/)

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
