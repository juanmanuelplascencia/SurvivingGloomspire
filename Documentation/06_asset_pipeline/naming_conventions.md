# Asset Naming Conventions

## Overview
This document defines the naming conventions for all assets in Surviving Gloomspire to ensure consistency and maintainability.

## General Rules

### Prefixes
All assets should be prefixed based on their type:
- `BP_` - Blueprint
- `DA_` - Data Asset
- `DT_` - Data Table
- `M_` - Material
- `MI_` - Material Instance
- `NS_` - Namespace (for organization)
- `PH_` - Placeholder
- `RT_` - Render Target
- `SM_` - Static Mesh
- `SK_` - Skeletal Mesh
- `T_` - Texture
- `UI_` - User Interface

### Naming Structure
```
[Prefix]_[AssetName]_[Variant]_[Resolution]
```

## Asset-Specific Conventions

### Textures
```
T_[AssetName]_[Type]_[Resolution]

Examples:
T_Character_Hero_Diffuse_2K
T_Env_Rock_Normal_1K
T_UI_Button_Play_Normal_512
```

#### Texture Types
- `_A` - Alpha
- `_AO` - Ambient Occlusion
- `_BC` - Base Color
- `_D` - Diffuse
- `_E` - Emissive
- `_M` - Metallic
- `_N` - Normal
- `_R` - Roughness
- `_ORM` - Occlusion/Roughness/Metallic (packed)

### Materials
```
M_[AssetName]_[Type]

Examples:
M_Character_Hero_Base
M_Env_Rock_Wet
M_UI_Button_Glowing
```

### Meshes
```
SM_[AssetName]_[Variant]_[LOD]
SK_[CharacterName]_[BodyPart]

Examples:
SM_Env_Rock_Large_01
SM_Prop_Chest_Wooden_01
SK_Hero_Body
SK_Enemy_Orc_Head
```

### Blueprints
```
BP_[Type]_[Name]_[Variant]

Examples:
BP_Character_Hero_Main
BP_Weapon_Sword_Magic
BP_Trigger_Checkpoint_Start
```

### UI Elements
```
UI_[Type]_[Name]_[State]

Examples:
UI_Button_StartGame_Normal
UI_Icon_Inventory_Selected
UI_ProgressBar_Health_Fill
```

### Animation
```
A_[Character]_[Action]_[Variant]

Examples:
A_Hero_Run_Forward
A_Enemy_Orc_Attack_01
A_NPC_Shopkeeper_Wave
```

### Audio
```
A_[Type]_[Category]_[Name]_[Variant]

Examples:
A_SFX_UI_ButtonClick_01
A_VOX_Enemy_Orc_Taunt_01
A_MUS_Combat_Boss_01
```

## Folder Structure

### Content Organization
```
Content/
├── Art/
│   ├── Characters/
│   ├── Environment/
│   └── UI/
├── Blueprints/
│   ├── Characters/
│   ├── Game/
│   └── UI/
├── Data/
│   ├── DataTables/
│   └── SaveGames/
└── Maps/
    ├── Levels/
    └── UI/
```

### Nested Folders
Use additional subfolders for better organization when needed:
```
Content/Art/Environment/
├── Props/
│   ├── Furniture/
│   └── Containers/
└── Architecture/
    ├── Walls/
    └── Floors/
```

## Version Control

### Source Assets
- Keep source files in a separate `SourceAssets` directory
- Follow similar naming conventions
- Include version numbers for WIP files

### Unreal Assets
- Never include temporary or WIP files in the main content folder
- Use Unreal's built-in version control for binary assets

## Best Practices

1. **Be Descriptive**
   - Use clear, descriptive names
   - Avoid abbreviations unless they're standard

2. **Be Consistent**
   - Follow the same pattern throughout the project
   - Document any exceptions

3. **Avoid Special Characters**
   - Use only letters, numbers, and underscores
   - No spaces or special characters

4. **Use PascalCase for Asset Names**
   - `HeroCharacter` not `hero_character` or `Hero_Character`

5. **Versioning**
   - Use `_v2`, `_v3`, etc. for asset versions
   - Keep previous versions until the new one is confirmed working

## Example Asset Tree
```
Content/
└── Art/
    ├── Characters/
    │   ├── Hero/
    │   │   ├── Textures/
    │   │   │   ├── T_Hero_BaseColor_2K
    │   │   │   └── T_Hero_Normal_2K
    │   │   └── Meshes/
    │   │       └── SK_Hero_Body
    │   └── Enemies/
    │       └── Orc/
    └── Environment/
        └── Props/
            └── SM_Prop_Chest_Wooden_01
```

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
