// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivingGloomspireTarget : TargetRules
{
    public SurvivingGloomspireTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        
        // Build configuration
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        
        // Module setup
        ExtraModuleNames.AddRange(new string[] { "SurvivingGloomspire" });
        
        // Build configuration
        bUseLoggingInShipping = true;
        
        // Platform specific settings
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // Android specific settings
            GlobalDefinitions.Add("WITH_MESH_SHADER_EMULATION=0");
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // iOS specific settings
            GlobalDefinitions.Add("WITH_MESH_SHADER_EMULATION=0");
        }
        
        // Enable exceptions
        bForceEnableExceptions = true;
        
        // Enable RTTI
        bForceEnableRTTI = true;
        
        // Build configuration
        bUseUnityBuild = true;
        bUsePCHFiles = true;
        
        // IWYU settings
        bEnforceIWYU = true;
        
        // Enable faster compile times
        bUseIncrementalLinking = true;
        bUseFastMonoCalls = true;
        bUseFastPDBLinking = true;
        
        // Enable faster iteration on non-shipping builds
        if (Configuration != UnrealTargetConfiguration.Shipping)
        {
            bUseIncrementalLinking = true;
            bUseFastPDBLinking = true;
            bUseFastMonoCalls = true;
            bUseUnityBuild = true;
            bUsePCHFiles = true;
        }
    }
}
