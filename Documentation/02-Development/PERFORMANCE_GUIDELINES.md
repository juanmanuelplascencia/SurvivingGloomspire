# Performance Guidelines

## CPU Optimization

### General Rules
- Profile before optimizing
- Focus on hot paths
- Avoid premature optimization
- Document performance-critical sections

### Memory Management
- Use object pooling
- Minimize dynamic allocations
- Pre-allocate containers
- Use appropriate data structures
- Avoid memory fragmentation

### Multi-threading
- Use task graph for parallel work
- Minimize lock contention
- Use lock-free patterns when appropriate
- Batch work for thread efficiency
- Profile thread utilization

## GPU Optimization

### Rendering
- Use instancing
- Implement LODs
- Optimize draw calls
- Use occlusion culling
- Profile GPU timings

### Shaders
- Minimize texture samples
- Use mipmaps
- Optimize shader math
- Use shader permutations wisely
- Profile shader performance

## Asset Guidelines

### Textures
- Use appropriate compression
- Implement texture atlases
- Use mipmaps
- Stream textures
- Monitor VRAM usage

### Meshes
- Optimize polygon count
- Use LODs
- Optimize UVs
- Reduce draw calls
- Use proper collision meshes

## Code Patterns to Avoid

### Common Pitfalls
- Unnecessary copies
- Excessive virtual calls
- String operations in hot paths
- Unbounded container growth
- Frequent memory allocations

### Better Alternatives
- Pass by const reference
- Use final/override
- Use string views
- Reserve container capacity
- Use object pools

## Performance Tools

### Built-in Profilers
- Unreal Insights
- GPU Visualizer
- Memory Profiler
- CPU Profiler
- Network Profiler

### External Tools
- RenderDoc
- PIX
- Nsight
- VTune
- Telemetry systems

## Optimization Checklist
- [ ] Profile the game
- [ ] Identify bottlenecks
- [ ] Implement optimizations
- [ ] Measure impact
- [ ] Verify no regressions
- [ ] Document changes

## Performance Budget
- **Frame Time**: 16.67ms (60 FPS)
- **CPU Time**: 10ms
- **GPU Time**: 6.67ms
- **Memory**: 8GB RAM max
- **VRAM**: 6GB max
- **Load Times**: 5s max (SSD)

## Best Practices
- Test on minimum spec hardware
- Monitor performance metrics
- Optimize based on real data
- Document performance decisions
- Review code for optimization opportunities
