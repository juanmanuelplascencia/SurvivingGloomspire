# Localization Workflow

## Overview
This document outlines the process for managing and implementing localization in Surviving Gloomspire, ensuring consistent translation and cultural adaptation across all supported languages.

## Supported Languages
- English (en) - Primary language
- Spanish (es) - Secondary language
- Additional languages TBD based on community interest

## File Structure

### Source Files
```
Content/
└── Localization/
    ├── Game/
    │   ├── en/
    │   │   └── Game.manifest
    │   │   └── Game.archive
    │   └── es/
    │       └── Game.manifest
    │       └── Game.archive
    └── UI/
        └── [Similar structure as above]
```

### Translation Files
```
SourceAssets/
└── Localization/
    ├── Game/
    │   ├── en/Game.po
    │   └── es/Game.po
    └── UI/
        └── [Similar structure as above]
```

## Text Management

### String Tables
- Use String Tables for all in-game text
- Organize by feature/context (e.g., `ST_UI_MainMenu`, `ST_Dialogue_NPCs`)
- Include context comments for translators

### Format
```ini
#. Context: Button tooltip in main menu
#. Character limit: 50
StartGame=Start Game
```

### Text Guidelines
1. **No Hardcoded Text**
   - All text must be in string tables
   - Use text formatting placeholders (e.g., `{0} of {1}`)

2. **Concise Writing**
   - Be brief but clear
   - Account for text expansion (30-50% for European languages)

3. **Variables**
   - Use named parameters: `Welcome back, {PlayerName}!`
   - Document variable types and constraints

## Translation Process

### 1. String Extraction
- Use Unreal's Localization Dashboard
- Export to .po format for translators
- Include context and character limits

### 2. Translation
- Professional translators work on .po files
- Review by native speakers
- Cultural adaptation check

### 3. Import & Testing
- Import translated .po files
- Verify in-game
- Check for text overflow
- Test special characters

## Technical Implementation

### Text Formatting
Use FText for all displayed text:
```cpp
FText WelcomeMessage = FText::Format(
    LOCTEXT("WelcomeMessage", "Welcome, {PlayerName}!"),
    TEXT("PlayerName"), PlayerName
);
```

### Dynamic Text
- Use FText::Format for dynamic content
- Keep format strings simple
- Document all placeholders

### Fonts
- Include necessary character sets
- Test with all supported languages
- Fallback font system

## Testing

### Automated Checks
- Missing translations
- Text overflow
- Unused strings
- Format string errors

### Manual Testing
- UI layout in all languages
- Text rendering
- Input methods
- Right-to-left (RTL) support if needed

## Voice Over

### File Naming
```
VO_[Character]_[Emotion]_[TextID]_[LanguageCode]

Example:
VO_Hero_Neutral_Greeting_01_EN
```

### Implementation
- Use Sound Cues with language-specific waves
- Implement subtitle system
- Sync with dialogue system

## Best Practices

### For Developers
1. Never concatenate localized strings
2. Use FText for all user-facing text
3. Test with pseudo-localization
4. Handle plural forms correctly

### For Translators
1. Maintain consistent terminology
2. Preserve HTML/formatting tags
3. Keep within character limits
4. Note cultural references

## Tools

### Unreal Engine
- Localization Dashboard
- String Table Editor
- Text History Tool

### External
- Poedit
- Crowdin (for community translations)
- Custom validation scripts

## Version Control

### Source Control
- Keep .po files in source control
- Use LFS for binary localization files
- Maintain change history for all translations

### Change Management
- Version all string changes
- Document context for new strings
- Deprecate rather than delete old strings

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
