# Audio Pipeline

## Overview
This document outlines the audio asset workflow and implementation standards for Surviving Gloomspire, ensuring consistent and efficient audio management.

## Supported Formats

### Primary Formats
- **Sound Effects**: `.wav` (PCM, 16/24-bit, 44.1kHz/48kHz)
- **Music**: `.wav` (PCM, 16/24-bit, 44.1kHz/48kHz)
- **Dialogue**: `.wav` (PCM, 16-bit, 22.05kHz/44.1kHz)

### Compression
- **SFX**: Ogg Vorbis (quality 80-100%)
- **Music**: Ogg Vorbis (quality 90-100%)
- **Dialogue**: Ogg Vorbis (quality 70-90%)

## Folder Structure

```
Content/
└── Audio/
    ├── SFX/
    │   ├── UI/
    │   ├── Gameplay/
    │   └── Ambience/
    ├── Music/
    │   ├── Combat/
    │   ├── Exploration/
    │   └── Menu/
    └── Voice/
        ├── Hero/
        └── NPCs/
```

## Naming Conventions

### General Format
```
A_[Type]_[Category]_[Name]_[Variant]_[Duration]
```

### Examples
- `A_SFX_UI_ButtonClick_01`
- `A_VOX_NPC_Shopkeeper_Greeting_01`
- `A_MUS_Combat_Boss_Phase1_3m42s`

## Audio Classes

### Sound Classes
1. **Master**
   - Volume: 1.0
   - Output Target: Main Output

2. **SFX**
   - Parent: Master
   - Volume: 0.8
   - Priority: 1.0

3. **Music**
   - Parent: Master
   - Volume: 0.7
   - Priority: 0.5

4. **Dialogue**
   - Parent: Master
   - Volume: 0.9
   - Priority: 2.0

## Implementation Guidelines

### Sound Cues
- Use Sound Cues for complex audio behaviors
- Keep cue graphs clean and well-organized
- Document special behaviors in the cue's description

### Attenuation
- Create presets for common attenuation settings
- Use spatialization for 3D sounds
- Set appropriate falloff distances

### Sound Mixes
- Create mixes for different game states
- Implement ducking for important sounds (e.g., dialogue)
- Use RTPCs for dynamic mixing

## Performance Optimization

### Voice Management
- Set appropriate voice limits per sound class
- Use priority to manage voice stealing
- Implement pooling for frequently played sounds

### Memory Management
- Stream large audio files (music, dialogue)
- Keep small, frequently played sounds in memory
- Monitor memory usage with Sound Wave memory budget

## Quality Standards

### Recording
- Record in a controlled environment
- Maintain consistent levels (-12dB to -6dB peak)
- Remove background noise and artifacts

### Processing
- Normalize to -3dB peak
- Apply light compression if needed (2:1 to 4:1 ratio)
- Use EQ to clean up frequencies

## Testing

### In-Engine Testing
- Test all sounds in context
- Verify 3D positioning
- Check for audio artifacts

### Platform Testing
- Test on all target platforms
- Verify compression quality
- Check memory usage

## Localization

### Voice Over
- Keep original recordings
- Organize by language code (e.g., `EN`, `ES`, `FR`)
- Include timestamps for lip-syncing

### Text-to-Speech
- Document any TTS usage
- Keep original script files
- Note any pronunciation guides

## Example Implementation

### Sound Cue Setup
```
+------------------+
| Sound Cue        |
| - Name: SC_Hit   |
+------------------+
        |
        v
+------------------+
| Random Node      |
| - Play Random    |
+------------------+
        |
        +--> [Hit_01]
        |
        +--> [Hit_02]
        |
        +--> [Hit_03]
```

### Attenuation Settings
```
Min Distance: 200 units
Max Distance: 4000 units
Falloff: Logarithmic
LPF: Enabled
Occlusion: Enabled
```

## Troubleshooting

### Common Issues
1. **No Sound**
   - Check volume sliders
   - Verify audio device selection
   - Check sound class routing

2. **Audio Artifacts**
   - Check for clipping
   - Verify sample rate consistency
   - Check for CPU spikes

3. **Performance Issues**
   - Monitor voice count
   - Check for memory leaks
   - Verify streaming settings

---
*Document Version: 1.0*  
*Last Updated: 2025-06-23*
