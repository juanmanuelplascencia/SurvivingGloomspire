# Performance Optimization Guidelines

## Overview
This document outlines best practices and guidelines for optimizing performance in the Unreal Engine 5 project. Following these guidelines will help maintain a consistent frame rate and smooth gameplay experience.

## Performance Targets

### Frame Rate
| Platform          | Target FPS | Minimum FPS |
|-------------------|------------|-------------|
| High-End PC       | 60 FPS     | 45 FPS      |
| Mid-Range PC      | 45 FPS     | 30 FPS      |
| Minimum Spec PC   | 30 FPS     | 20 FPS      |
| Console (Next-Gen)| 60 FPS     | 45 FPS      |


### Memory Budgets
| Resource Type        | Budget (High-End) | Budget (Minimum Spec) |
|----------------------|-------------------|------------------------|
| Total RAM Usage      | 8 GB             | 4 GB                  |
| VRAM Usage          | 6 GB             | 2 GB                  |
| Level Streaming     | 1.5 GB           | 500 MB                |
| Audio Memory        | 256 MB           | 128 MB                |
| Texture Memory      | 4 GB             | 1 GB                  |


## Optimization Techniques

### 1. Rendering Optimization

#### Level of Detail (LOD)
- Implement LODs for all static and skeletal meshes
- Configure LOD distances appropriately
- Use automatic LOD generation tools

```ini
; DefaultEngine.ini
[/Script/Engine.StaticMesh]
MinLOD=0
bSupportUniformlyDistributedSampling=True

[/Script/Engine.SkeletalMesh]
MinLOD=0
bEnablePerPolyCollision=False
```

#### Culling
- Use culling distances for actors and components
- Implement occlusion culling
- Use hierarchical LOD (HLOD) for complex scenes

```cpp
// Set culling distances
StaticMeshComponent->SetCullDistance(10000.0f);
StaticMeshComponent->SetCachedMaxDrawDistance(15000.0f);

// Enable distance culling
StaticMeshComponent->SetCullDistanceVolumes(true);
StaticMeshComponent->bAllowCullDistanceVolume = true;
```

#### Shader Optimization
- Use material instances instead of dynamic material parameters
- Minimize texture samples
- Use material functions for reusable shader code
- Optimize shader complexity

```cpp
// Check shader complexity
static TAutoConsoleVariable<int32> CVarShaderComplexity(
    TEXT("r.ShaderComplexity"),
    0,
    TEXT("Enables shader complexity view mode")
);
```

### 2. CPU Optimization

#### Tick Optimization
- Disable tick on actors when not needed
- Use timers instead of tick for infrequent updates
- Implement custom tick groups for better scheduling

```cpp
// Disable tick by default
PrimaryActorTick.bCanEverTick = false;
PrimaryActorTick.bStartWithTickEnabled = false;

// Enable/disable tick when needed
SetActorTickEnabled(true);
```

#### Blueprint Optimization
- Minimize Blueprint tick usage
- Use event-based programming
- Avoid complex logic in Blueprint tick
- Use Blueprint nativization for performance-critical Blueprints

```ini
; DefaultGame.ini
[/Script/UnrealEd.ProjectPackagingSettings]
+BlueprintNativizationMethod=Exclusive
+BlueprintNativizationMethod=Inclusive
```

#### Garbage Collection
- Minimize UObject allocations during gameplay
- Use object pooling for frequently created/destroyed objects
- Manually trigger garbage collection during loading screens

```cpp
// Force garbage collection
GEngine->ForceGarbageCollection(true);

// Set GC parameters
GEngine->GetEngineUserSettings()->SetGarbageCollectionSettings(
    /*bPermanentObjectPool=*/ true,
    /*MaxObjectsInGame=*/ 100000,
    /*MaxObjectsInEditor=*/ 200000,
    /*MaxObjectsInPIE=*/ 150000
);
```

### 3. Memory Optimization

#### Texture Optimization
- Use appropriate texture sizes
- Implement texture streaming
- Use texture atlases
- Compress textures appropriately

```ini
; DefaultEngine.ini
[/Script/Engine.TextureLODSettings]
+TextureLODGroups=(Group=TEXTUREGROUP_World, MinLODSize=1, MaxLODSize=4096, LODBias=0, NumStreamedMips=3)
+TextureLODGroups=(Group=TEXTUREGROUP_Character, MinLODSize=1, MaxLODSize=2048, LODBias=0, NumStreamedMips=3)
```

#### Mesh Optimization
- Optimize polygon count
- Use LODs
- Simplify collision geometry
- Use instanced static meshes for repeated geometry

```cpp
// Create instanced static mesh component
UInstancedStaticMeshComponent* InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMesh"));
InstancedMesh->SetStaticMesh(MyStaticMesh);
InstancedMesh->SetMaterial(0, MyMaterial);
InstancedMesh->AddInstance(FTransform::Identity);
```

### 4. Network Optimization

#### Replication
- Minimize replicated properties
- Use replication conditions
- Implement relevancy checks
- Use RPCs efficiently

```cpp
// Replicate only to relevant clients
DOREPLIFETIME_CONDITION(AMyActor, Health, COND_OwnerOnly);

// Implement relevancy check
bool AMyActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
    // Custom relevancy logic
    return IsValid(ViewTarget) && FVector::Dist(GetActorLocation(), ViewTarget->GetActorLocation()) < 10000.0f;
}
```

#### Bandwidth Optimization
- Compress replicated data
- Implement delta compression
- Use property replication conditions
- Optimize RPC parameters

```cpp
// Replicate with compression
UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, Category = "Stats")
int32 Health;

// Replicate only if changed by more than 1.0
UPROPERTY(ReplicatedUsing = OnRep_Stamina, EditAnywhere, Category = "Stats", ReplicatedUsing = OnRep_Stamina)
float Stamina;

UFUNCTION()
void OnRep_Stamina()
{
    // Handle replication
}
```

## Performance Profiling

### Built-in Profiling Tools

#### Console Commands
```
# Basic performance stats
stat unit
stat unitgraph
stat game
stat engine

# GPU profiling
stat scenerendering
stat initviews
stat shadowrendering

# Memory stats
stat memory
memreport -full

# Detailed stats
stat scenerendering -density
stat shadowrendering -density

# Profile GPU
profilegpu

# Profile CPU
profilecpu
```

#### Unreal Insights
1. Launch Unreal Insights with your game
2. Capture performance data
3. Analyze frame times, CPU/GPU usage, and memory
4. Identify performance bottlenecks

### Optimization Workflow

1. **Profile**
   - Identify performance bottlenecks
   - Use profiling tools to measure impact
   - Set performance budgets

2. **Optimize**
   - Apply optimization techniques
   - Test changes in isolation
   - Measure performance impact

3. **Verify**
   - Test on multiple hardware configurations
   - Check for visual or gameplay regressions
   - Validate against performance targets

4. **Document**
   - Record optimization changes
   - Update performance guidelines
   - Share knowledge with the team

## Platform-Specific Optimization

### PC
- Support multiple quality presets
- Implement dynamic resolution scaling
- Optimize for different hardware configurations

### Console
- Optimize for fixed hardware
- Implement memory optimization techniques
- Follow platform-specific guidelines

### Mobile
- Minimize draw calls
- Optimize shaders for mobile
- Reduce memory usage
- Implement battery-saving features

## Common Performance Issues

### 1. High CPU Usage
- **Causes**:
  - Excessive tick events
  - Complex Blueprint logic
  - Inefficient algorithms
- **Solutions**:
  - Optimize tick usage
  - Move logic to C++
  - Use profiling to identify hotspots

### 2. High GPU Usage
- **Causes**:
  - Complex shaders
  - High-resolution textures
  - Excessive post-processing
- **Solutions**:
  - Optimize materials
  - Reduce texture sizes
  - Adjust post-processing settings

### 3. Memory Leaks
- **Causes**:
  - Unreleased resources
  - Circular references
  - Large asset loads
- **Solutions**:
  - Use memory profiler
  - Implement proper cleanup
  - Use smart pointers

## Best Practices

### General
- Profile early and often
- Set performance budgets
- Test on target hardware
- Document optimization decisions

### Code
- Use const correctness
- Prefer stack allocation over heap
- Minimize virtual function calls
- Use data-oriented design for performance-critical code

### Content
- Optimize assets for target platform
- Use appropriate LODs
- Implement streaming where necessary
- Test with different hardware configurations

## Performance Checklist

### Before Committing Code
- [ ] Profile changes
- [ ] Check for memory leaks
- [ ] Verify performance on minimum spec
- [ ] Document any performance implications

### Before Release
- [ ] Run full performance test suite
- [ ] Verify memory usage
- [ ] Check for hitches or stalls
- [ ] Validate against performance targets

## Related Documents
- [Project Setup Guide](project_setup_guide.md)
- [Content Guidelines](content_guidelines.md)
- [Unreal Engine Optimization Documentation](https://docs.unrealengine.com/optimization/)

---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
