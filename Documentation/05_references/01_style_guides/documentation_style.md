# Documentation Style Guide

## General Guidelines

### File Naming
- Use lowercase with hyphens for multi-word filenames
- Be descriptive but concise
- Examples:
  - `combat-system.md` (good)
  - `CombatSystem.md` (avoid)
  - `combat_system.md` (acceptable)

### Document Structure
Each document should follow this structure:
```markdown
# Document Title

## Section
Content

### Subsection
More detailed content

---
*Document Version: X.Y*  
*Last Updated: YYYY-MM-DD*
```

## Markdown Formatting

### Headers
```markdown
# H1 - Document Title
## H2 - Major Section
### H3 - Subsection
#### H4 - Sub-subsection
```

### Text Formatting
- **Bold** for UI elements and important concepts
- *Italic* for emphasis or game terms
- `Code` for file names, functions, and code references

### Lists
Use ordered lists for sequential steps, unordered for other cases:

```markdown
1. First step
2. Second step
   - Sub-point
   - Another sub-point
```

### Tables
Use tables for comparing options or displaying structured data:

| Property | Type | Description |
|----------|------|-------------|
| Health   | int  | Player's health points |
| Mana     | int  | Magic points |

### Code Blocks
Use language-specific code fences:

```cpp
void ExampleFunction()
{
    // Code here
}
```

## Writing Style

### Voice and Tone
- Use active voice
- Be concise but clear
- Address the reader as "you" in tutorials
- Use "we" when referring to the development team

### Terminology
- Be consistent with game-specific terms
- Define new terms on first use
- Use industry-standard terminology when possible

### Images and Diagrams
- Place in `/Documentation/Assets/Images/`
- Use descriptive filenames
- Include alt text for accessibility
- Recommended formats:
  - PNG for screenshots/diagrams
  - SVG for vector graphics

## Version Control

### Commit Messages
- Use present tense
- Start with a verb
- Be specific but concise
- Reference issues when applicable

Example:
```
Add player movement controls

- Implement basic WASD movement
- Add movement animations
- Fix collision detection

Closes #123
```

## Review Process
1. Self-review for:
   - Spelling and grammar
   - Broken links
   - Formatting consistency
2. Peer review for:
   - Technical accuracy
   - Clarity
   - Completeness
3. Update version number and date

## Document Types

### Technical Specifications
- Focus on implementation details
- Include diagrams and flowcharts
- Document edge cases

### User Guides
- Step-by-step instructions
- Include screenshots
- Common issues and solutions

### API Documentation
- Function/method signatures
- Parameter descriptions
- Return values
- Examples

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
