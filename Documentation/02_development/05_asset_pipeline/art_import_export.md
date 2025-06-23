# Art Import/Export Pipeline

## Overview
This document outlines the workflow for importing and managing art assets in Unreal Engine 5 for Surviving Gloomspire.

## Supported File Formats

### Textures
- **Base Color/Albedo**: `.png`, `.tga` (sRGB, 8/16-bit)
- **Normal Maps**: `.tga` (Linear, 8/16-bit, DirectX)
- **Roughness/Metallic/AO**: `.png` (Linear, 8-bit, grayscale)

### 3D Models
- **Static Meshes**: `.fbx`, `.obj`
- **Skeletal Meshes**: `.fbx`
- **Animation**: `.fbx`

### 2D/UI
- **Sprites**: `.png` (RGBA, power of 2 dimensions)
- **UI Elements**: `.psd`, `.png` (with layers preserved)

## Folder Structure

```
Content/
├── Art/
│   ├── Characters/
│   │   ├── Hero/
│   │   │   ├── Meshes/
│   │   │   ├── Textures/
│   │   │   └── Materials/
│   │   └── NPCs/
│   │
│   ├── Environment/
│   │   ├── Props/
│   │   └── Architecture/
│   │
│   └── UI/
│       ├── Icons/
│       └── Menus/
```

## Import Settings

### Textures
- **sRGB**: Enabled for color textures, disabled for data (normal, roughness, etc.)
- **Compression**:
  - Color: BC7 (HDR: BC6H)
  - Normal: BC5
  - Grayscale: BC4
- **Mip Maps**: Enabled for 3D, disabled for UI

### Static Meshes
- **Generate Lightmap UVs**: Enabled
- **Combine Meshes**: Disabled (import separately)
- **Auto Generate Collision**: Disabled (create custom collision)

### Skeletal Meshes
- **Import Morph Targets**: Enabled if needed
- **Use T0 as Ref Pose**: Enabled
- **Preserve Smoothing Groups**: Enabled

## Naming Conventions

### Textures
```
T_[AssetName]_[Type]_[Resolution]

Example:
T_Hero_Diffuse_2K
T_Env_Rock_Normal_4K
```

### Materials
```
M_[AssetName]_[Type]

Example:
M_Hero_Base
M_Env_Rock_Wet
```

### Meshes
```
SM_[AssetName]_[LOD]
SK_[CharacterName]_[BodyPart]

Examples:
SM_Rock_Large_01
SK_Hero_Body
```

## Export Guidelines

### From 3D Software
1. Freeze transformations
2. Center pivot
3. Apply scale (1,1,1)
4. Triangulate meshes
5. Check UVs

### From Image Editors
1. Remove unnecessary alpha channels
2. Save at appropriate resolution
3. Use lossless compression when possible

## Version Control
- All source files (`.psd`, `.blend`, `.max`, etc.) should be kept in a separate `SourceAssets` directory
- Only final exported assets should be in the `Content` folder
- Use `.gitattributes` to handle binary files with LFS

## Performance Considerations
- **Texture Resolution**:
  - Characters: 2K
  - Environment: 1K-4K based on importance
  - Small props: 512px or less
- **Polygon Count**:
  - Hero characters: 30-50K triangles
  - NPCs: 5-15K triangles
  - Props: 500-5K triangles

## Validation Checklist
- [ ] No N-gons (only triangles and quads)
- [ ] UVs within 0-1 space
- [ ] Proper pivot/origin placement
- [ ] Correct scale (1 unit = 1cm in Unreal)
- [ ] No unnecessary geometry
- [ ] Proper material assignments

## Troubleshooting

### Common Issues
1. **Incorrect Scale**
   - Check export units match Unreal's (cm)
   - Verify scale factor in import settings

2. **Missing Textures**
   - Ensure texture files are in expected locations
   - Check texture naming matches material references

3. **Import Errors**
   - Check for unsupported features
   - Try re-exporting with different settings

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
