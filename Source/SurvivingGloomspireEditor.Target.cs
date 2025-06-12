// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivingGloomspireEditorTarget : TargetRules
{
    public SurvivingGloomspireEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        
        // Build configuration
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        
        // Module setup
        ExtraModuleNames.AddRange(new string[] { "SurvivingGloomspire" });
        
        // Editor specific settings
        bBuildDeveloperTools = true;
        bBuildWithEditorOnlyData = true;
        bCompileRecast = true;
        
        // Build configuration
        bUseUnityBuild = true;
        bUsePCHFiles = true;
        
        // IWYU settings
        bEnforceIWYU = true;
        
        // Enable faster compile times
        bUseIncrementalLinking = true;
        bUseFastMonoCalls = true;
        bUseFastPDBLinking = true;
        
        // Enable live coding
        bUseAdaptiveUnityBuild = true;
        bUseUnityBuild = true;
        
        // Enable faster iteration on non-shipping builds
        if (Configuration != UnrealTargetConfiguration.Shipping)
        {
            bUseIncrementalLinking = true;
            bUseFastPDBLinking = true;
            bUseFastMonoCalls = true;
            bUseUnityBuild = true;
            bUsePCHFiles = true;
            
            // Enable live coding
            bUseAdaptiveUnityBuild = true;
            bUseUnityBuild = true;
        }
    }
}
