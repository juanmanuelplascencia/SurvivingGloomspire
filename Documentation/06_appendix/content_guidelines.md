# Content Creation Guidelines

## Overview
This document provides guidelines for creating and managing game content to ensure consistency, quality, and performance across the project.

## Asset Naming Conventions

### General Rules
- Use PascalCase for all asset names (e.g., `HeroCharacter`, `FireballSpell`)
- Use descriptive names that indicate purpose and type
- Prefix asset names with type abbreviation (see table below)
- Use underscores only to separate type prefixes from names
- Avoid special characters and spaces in file names

### Asset Type Prefixes
| Asset Type          | Prefix | Example                     |
|---------------------|--------|-----------------------------|
| Character           | CH_    | `CH_Hero_Warrior`           |
| Enemy               | EN_    | `EN_Goblin_Melee`           |
| Weapon              | WP_    | `WP_Sword_Long`             |
| Armor               | AR_    | `AR_Plate_Chest`            |
| Item               | IT_    | `IT_Potion_Health`          |
| Ability            | AB_    | `AB_Fireball`               |
| Spell              | SP_    | `SP_HealingWord`            |
| Particle System    | PS_    | `PS_FireExplosion`          |
| Material           | M_     | `M_Metal_Rusty`             |
| Material Instance  | MI_    | `MI_Metal_Rusty_Worn`       |
| Texture            | T_     | `T_Character_Hero_Diffuse`  |
| Sound              | S_     | `S_Explosion_Fire`          |
| Animation          | A_     | `A_Character_Run`           |
| Animation Blueprint| ABP_   | `ABP_Character_Base`        |
| Blueprint          | BP_    | `BP_Door_Interactive`       |
| Level              | L_     | `L_Village_Main`            |
| UI Widget          | W_     | `W_Inventory_Slot`          |
| Data Asset         | DA_    | `DA_Item_Weapons`           |
| Data Table         | DT_    | `DT_Item_Weapons`           |


## Folder Structure

### Core Directories
```
Content/
├── Characters/              # Playable and non-playable characters
│   ├── Player/             # Player characters
│   ├── Enemies/            # Enemy characters
│   └── NPCs/               # Non-player characters
│
├── Gameplay/              # Core gameplay elements
│   ├── Abilities/          # Gameplay abilities
│   ├── Items/              # Inventory items
│   ├── Weapons/            # Weapons and equipment
│   └── Effects/            # Gameplay effects
│
├── Environment/           # Level assets
│   ├── Props/              # Interactive props
│   ├── Architecture/       # Building pieces
│   ├── Foliage/            # Plants and vegetation
│   └── Lighting/           # Light setups
│
├── UI/                    # User interface
│   ├── HUD/                # Heads-up display
│   ├── Menus/              # Game menus
│   ├── Icons/              # UI icons
│   └── Fonts/              # Font assets
│
├── Audio/                 # Sound assets
│   ├── Music/              # Background music
│   ├── SFX/                # Sound effects
│   └── Dialogue/           # Voice lines
│
└── VFX/                   # Visual effects
    ├── Particles/          # Particle systems
    ├── Materials/          # Effect materials
    └── PostProcess/        # Post-processing effects
```

## Asset Creation Guidelines

### Textures
- **Resolution**: Use power-of-two dimensions (e.g., 256x256, 512x512, 1024x1024)
- **Compression**:
  - Diffuse/Albedo: BC3/DXT5 (RGBA) or BC1/DXT1 (RGB)
  - Normal Maps: BC5/3Dc or BC7
  - Masks/Packed: BC4 (grayscale) or BC5 (2-channel)
- **Mip Maps**: Enable for all textures except UI elements
- **sRGB**: Enable for color textures, disable for data textures (normal, roughness, etc.)

### Materials
- **Instancing**: Use material instances for variations
- **Parameters**: Expose commonly changed values as parameters
- **Performance**:
  - Limit texture samples
  - Use material functions for reuse
  - Avoid complex math in pixel shaders
- **Naming**:
  - Base Material: `M_Base_Type`
  - Instance: `MI_Type_Variation`

### Static Meshes
- **Polygon Count**:
  - Hero Assets: 5,000-20,000 triangles
  - Environment Assets: 500-5,000 triangles
  - Distant/Foliage: 50-500 triangles
- **LODs**:
  - LOD0: 100%
  - LOD1: 50%
  - LOD2: 25%
  - LOD3: 12.5%
- **Collision**: Use simple collision shapes when possible
- **Lightmap UVs**: Generate unique UV channel (typically UV1)

### Skeletal Meshes
- **Bone Limits**:
  - Main Character: ~150 bones
  - Enemies: ~100 bones
  - Creatures: 50-200 bones
- **LODs**: Similar to static meshes, but with skeleton reduction
- **Morph Targets**: For facial animation and customization
- **Physics Assets**: For ragdoll and physical animation

### Animation
- **Frame Rate**: 30 FPS for gameplay, 15 FPS for distant characters
- **Root Motion**: Use for precise character movement
- **Blend Spaces**: For smooth transitions between animations
- **Animation Blueprints**: For complex animation logic
- **Notify States**: For sound and effect triggers

### Blueprints
- **Organization**:
  - Use functions for reusable logic
  - Group related nodes
  - Add comments for complex logic
- **Performance**:
  - Use event tick sparingly
  - Use timers for delayed actions
  - Avoid heavy operations in tick
- **Communication**:
  - Use interfaces for Blueprint communication
  - Use event dispatchers for one-to-many communication

## Level Design Guidelines

### Blockout Phase
1. Use BSP or simple static meshes for blockout
2. Establish scale and proportions
3. Test gameplay flow
4. Iterate based on playtesting

### Lighting
- **Static Lighting**: For static geometry
- **Stationary Lights**: For dynamic objects
- **Dynamic Lights**: For moveable objects
- **Lightmap Resolution**:
  - Small objects: 64-128
  - Medium objects: 128-256
  - Large objects: 256-1024
  - Terrain: 4-16

### Optimization
- **Visibility Culling**: Use culling volumes
- **Level Streaming**: For large levels
- **LODs**: For all meshes
- **Occlusion Culling**: For complex scenes
- **Lightmap Optimization**: Adjust lightmap resolution and quality

## Audio Guidelines

### File Formats
- **Music**: .wav (uncompressed) or .ogg (compressed)
- **SFX**: .wav (uncompressed)
- **Dialogue**: .wav (16-bit, 48kHz)

### Categories
- **Master**: Overall volume
- **SFX**: Sound effects
- **Music**: Background music
- **Dialogue**: Character voices
- **Ambience**: Environmental sounds

### Implementation
- Use sound cues for complex audio
- Implement audio attenuation
- Use sound classes for volume control
- Implement reverb zones

## Localization

### Text
- Use text variables for all UI text
- Implement string tables for localization
- Leave room for text expansion (typically +30%)
- Test with different languages

### Assets
- Separate culture-specific assets
- Use culture-specific audio
- Consider text direction (RTL/LTR)

## Quality Assurance

### Testing Checklist
- [ ] Assets load correctly
- [ ] Textures are not stretched
- [ ] Materials display correctly
- [ ] Animations play smoothly
- [ ] Sounds play at appropriate volumes
- [ ] Performance is within target
- [ ] No console errors or warnings

### Performance Budget
- **Frame Time**: 16.67ms (60 FPS) target, 33.33ms (30 FPS) minimum
- **Draw Calls**: <1000 per frame
- **Texture Memory**: <1GB for all textures
- **Vertex Count**: <1 million visible vertices
- **Particles**: <10,000 particles per frame

## Submission Process

1. **Asset Review**
   - Follow naming conventions
   - Check for optimization
   - Verify textures and materials

2. **Version Control**
   - Use descriptive commit messages
   - Include relevant issue numbers
   - Keep commits atomic

3. **Documentation**
   - Update asset lists
   - Document any special requirements
   - Add usage instructions if needed

## Revision History

| Version | Date       | Author          | Changes                        |
|---------|------------|-----------------|--------------------------------|
| 1.0     | 2023-11-15 | [Your Name]     | Initial version                |


## Related Documents
- [Project Setup Guide](project_setup_guide.md)
- [Performance Guidelines](performance_guidelines.md)
- [UI Style Guide](ui_style_guide.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
