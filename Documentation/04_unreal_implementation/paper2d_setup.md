# 2D Setup with Paper2D

## Overview
This document covers the setup and usage of Unreal Engine 5's Paper2D system for implementing 2D characters, animations, and environments in Pathfinder 1e.

## Getting Started with Paper2D

### Enabling Paper2D
1. Open your project in Unreal Engine 5
2. Go to Edit > Plugins
3. Enable "Paper2D" under the "2D" section
4. Restart the editor when prompted

### Project Settings
1. Open Project Settings (Edit > Project Settings)
2. Navigate to Engine - Rendering
3. Ensure "2D" is enabled under "Rendering"
4. Set "Default Opaque Blend Mode" to "Masked" for proper sprite rendering

## Sprite Creation

### Importing Sprite Sheets
1. Prepare your sprite sheet with a transparent background (PNG recommended)
2. In the Content Browser, right-click and select Import
3. Choose your sprite sheet file
4. In the import options:
   - Set "Texture Group" to "UI" for UI elements or "Sprites" for game sprites
   - Enable "Flip Green Channel" if your normal maps appear inverted
   - Set "Compression Settings" to "UserInterface2D (RGBA)" for crisp sprites

### Creating Sprite Assets
1. Right-click on your imported texture
2. Select "Sprite Actions > Extract Sprites"
3. Adjust the grid size or use the auto-detect feature
4. Select and name individual sprites
5. Click "Extract" to create sprite assets

## Character Setup

### Creating a 2D Character
1. Create a new Blueprint class that inherits from `PaperCharacter`
2. In the Blueprint editor, add a `PaperFlipbookComponent` for the character's visual
3. Set up collision:
   - Add a `CapsuleComponent` for character collision
   - Adjust the capsule size to match your sprite
   - Set the sprite to render in front of the capsule

### Animation Setup
1. Create a new `PaperFlipbook` asset
2. Add frames to the flipbook from your sprite assets
3. Set the frames per second (FPS) for the animation
4. Create animation assets for each character state (Idle, Walk, Attack, etc.)

### Animation Blueprint
1. Create a new Animation Blueprint for your character
2. Set the target skeleton to your character's skeleton (if using)
3. Create state machines for different animation states
4. Use variables to control transitions between states

## Tile Maps

### Creating a Tile Set
1. Import your tile set texture
2. Right-click the texture and select "Create > Tileset"
3. Set the tile size and margin/padding
4. Define collision for each tile if needed

### Creating a Tile Map
1. Create a new `PaperTileMap` asset
2. Set the tile map size in tiles
3. Paint tiles onto the grid
4. Add the tile map to your level

## Camera Setup

### 2D Camera
1. Add a `CameraComponent` to your character or create a separate camera actor
2. Set the camera to orthographic mode
3. Adjust the orthographic width for the desired zoom level
4. Enable "Constrain Aspect Ratio" for consistent scaling

### Camera Bounds
1. Create a `BoxComponent` to define the camera bounds
2. Use this to prevent the camera from showing outside the playable area
3. Adjust the box size to match your level boundaries

## Input Handling

### 2D Movement
1. Set up axis mappings in Project Settings > Input
2. Create a movement component or use the built-in `CharacterMovementComponent`
3. Handle input in your character's Blueprint or C++

### Example: Basic 2D Movement
```cpp
// In your character class header
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
class UPaperFlipbookComponent* SpriteComponent;

// In your character class implementation
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Bind movement axis
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
    
    // Bind actions
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::StartAttack);
}

void AMyCharacter::MoveRight(float Value)
{
    // Apply movement
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    
    // Update sprite facing
    if (Value != 0.0f)
    {
        const bool bIsFacingRight = Value > 0.0f;
        SetActorRotation(bIsFacingRight ? FRotator(0.0f, 0.0f, 0.0f) : FRotator(0.0f, 180.0f, 0.0f));
        
        // Play walk animation
        if (!bIsAttacking)
        {
            PlayAnimation(EWalkAnimation);
        }
    }
    else if (!bIsAttacking)
    {
        // Play idle animation
        PlayAnimation(EIdleAnimation);
    }
}
```

## Sorting and Layers

### Z-Ordering
1. Use the `SetRelativeLocation` Z-coordinate to control draw order
2. Higher Z values are drawn on top of lower ones
3. For dynamic sorting, use the `SortOrder` property on sprite components

### Example: Dynamic Z-Ordering
```cpp
void AMyCharacter::UpdateSortOrder()
{
    // Base Z position on the character's Y position for pseudo-3D depth
    const float NewZ = -GetActorLocation().Y;
    SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, NewZ));
    
    // Update sprite sort order
    if (SpriteComponent)
    {
        SpriteComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f)); // Slight offset
        SpriteComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
        SpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    }
}
```

## Performance Optimization

### Sprite Atlases
1. Combine multiple sprites into a single texture atlas
2. Use the Sprite Atlas asset to manage sprite sheets
3. Reduces draw calls and improves performance

### Level of Detail (LOD)
1. Create lower resolution versions of sprites for distant objects
2. Set up LOD settings in the sprite properties
3. Adjust LOD distances based on performance needs

### Culling
1. Use `PaperSpriteComponent`'s culling settings
2. Set appropriate bounds for sprites
3. Consider using `SetCullDistance` for performance optimization

## Debugging

### Debug Drawing
```cpp
// Enable collision debugging
void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Draw debug bounds
    if (bShowDebug)
    {
        DrawDebugBox(GetWorld(), GetActorLocation(), 
                    FVector(50.0f, 50.0f, 50.0f), 
                    FColor::Green, 
                    false, -1.0f, 0, 2.0f);
    }
}
```

### Common Issues
1. **Sprites not appearing**: Check sprite import settings and material
2. **Incorrect sorting**: Adjust Z-order or sort order values
3. **Animation not playing**: Verify animation assets and Blueprint logic
4. **Performance issues**: Use stat unit and stat game to identify bottlenecks

## Best Practices

### Organization
1. Keep sprite assets organized in folders by type (Characters, UI, Environment)
2. Use consistent naming conventions (e.g., `CH_CharacterName_Idle`, `ENV_Tree_01`)
3. Use subfolders for different character states and animations

### Asset Management
1. Use the Paper2D editor for sprite editing
2. Set up proper collision presets
3. Use sprite sockets for attaching weapons or effects

### Performance
1. Minimize overdraw by using sprite atlases
2. Use sprite instancing for repeated elements
3. Profile regularly to identify performance bottlenecks

## Related Documents
- [Gameplay Ability System](gameplay_ability_system.md)
- [Data Management](data_management.md)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
