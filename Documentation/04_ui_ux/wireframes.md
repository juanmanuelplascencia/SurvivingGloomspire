# UI Wireframes

## Overview
This document contains wireframes for the key screens in Surviving Gloomspire. These wireframes serve as a visual guide for the UI layout and flow.

## Wireframe Key
- `[T]` - Text
- `[B]` - Button
- `[I]` - Image/Icon
- `[F]` - Form Field
- `[M]` - Menu
- `[+]` - Expandable Section

## 1. Main Menu
```
+-------------------------------------+
|                                     |
|  [I] SURVIVING GLOOMSPIRE LOGO [I]  |
|                                     |
|                                     |
|           [B] NEW GAME              |
|           [B] LOAD GAME             |
|           [B] OPTIONS              |
|           [B] QUIT                 |
|                                     |
|  [T] Version 1.0.0              |
+-------------------------------------+
```

## 2. Character Creation - Race Selection
```
+-------------------------------------+
| [< BACK]  CHARACTER CREATION  (1/7) |
+------------------+------------------+
| [I] Human        | [T] Human        |
| [I] Elf          | [T] +2 to one   |
| [I] Dwarf        |    ability score |
| [I] Halfling     | [T] Medium size  |
| [I] Gnome        | [T] 30ft speed  |
| [I] Half-Elf     | [T] Common lang  |
| [I] Half-Orc     |                  |
|                  |                  |
+------------------+  [B] SELECT RACE  |
| [B] CUSTOMIZE APPEARANCE > |
+-------------------------------------+
```

## 3. In-Game HUD
```
+-------------------------------------+
| [I] HP: 45/60 [=======    ]   [I] |
| [I] MP: 12/40 [===       ]   [M] |
|                                     |
|                [Minimap]            |
|                [Compass]            |
+-------------------------------------+
|                                     |
|                                     |
|            [Context Menu]           |
|              [B] Attack             |
|              [B] Use Item           |
|              [B] Cast Spell         |
|                                     |
+-------------------------------------+
| [1][2][3][4][5] [6][7][8][9][0]     |
+-------------------------------------+
| [B]CHAR  [B]INV  [B]SPELL  [B]JOURN |
+-------------------------------------+
```

## 4. Character Sheet
```
+-------------------------------------+
| [< BACK]  CHARACTER SHEET           |
+------------------+------------------+
| [I] Avatar       | [T] Name: [F]    |
|                  | [T] Race: [F]    |
|                  | [T] Class: [F]   |
|                  | [T] Level: [F]   |
+------------------+------------------+
| [B] ATTRIBUTES   | [B] SKILLS       |
+------------------+------------------+
| STR: 16 (+3)     | Acrobatics +5    |
| DEX: 14 (+2)     | Arcana +2        |
| CON: 12 (+1)     | Athletics +6     |
| INT: 10 (+0)     | Crafting +1      |
| WIS: 14 (+2)     | Deception +3     |
| CHA: 16 (+3)     | Diplomacy +6     |
+------------------+------------------+
| [B] INVENTORY    | [B] SPELLS       |
+------------------+------------------+
```

## 5. Inventory Screen
```
+-------------------------------------+
| [< BACK]  INVENTORY (45/60 lbs)     |
+------------------+------------------+
| [I] Longsword    | [I] Chain Mail   |
| [T] 1d8+3 Slash | [T] AC +6        |
| [T] 2 lbs       | [T] 40 lbs       |
|                  |                  |
| [I] Health Potion| [I] Rope (50ft)  |
| [T] Heal 2d4+2  | [T] 5 lbs       |
| [T] 0.5 lbs     |                  |
|                  |                  |
+------------------+------------------+
| [B] EQUIPMENT  | [B] CONSUMABLES  |
| [B] QUESTS     | [B] JUNK        |
+-------------------------------------+
```

## 6. Dialogue Screen
```
+-------------------------------------+
| [I] NPC Portrait                   |
|                                     |
| [T] Greetings, traveler! What     |
|     brings you to these parts?      |
|                                     |
+-------------------------------------+
| [B] 1. I'm looking for work.       |
| [B] 2. Just passing through.       |
| [B] 3. Do you know anything about  |
|     the missing caravan?           |
| [B] 4. [Intimidation] Tell me what |
|     you know, or else!             |
+-------------------------------------+
| [B] [Persuasion] [Bluff] [Lore]    |
+-------------------------------------+
```

## 7. Journal/Quest Log
```
+-------------------------------------+
| [< BACK]  JOURNAL                   |
+------------------+------------------+
| [B] ACTIVE       | [T] Main Quest:  |
| [B] COMPLETED    | Defeat the Lich  |
| [B] FAILED       |                  |
|                  | [T] - Find the   |
|                  |     ancient tomb  |
|                  | [T] - Recover the|
|                  |     lost amulet   |
|                  | [T] - Confront   |
|                  |     the lich      |
|                  |                  |
|                  | [T] Side Quest:   |
|                  | Missing Caravan   |
+-------------------------------------+
```

## Implementation Notes

### 1. Responsive Design
- UI scales based on resolution
- Elements reposition for different aspect ratios
- Touch targets sized for both mouse and touch input

### 2. Navigation
- Consistent back/cancel button placement
- Clear visual hierarchy
- Breadcrumb navigation for multi-step processes

### 3. Feedback
- Visual and audio feedback for all actions
- Loading indicators for async operations
- Clear error messages

### 4. Accessibility
- Keyboard navigation support
- Screen reader compatibility
- Adjustable text sizes

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
