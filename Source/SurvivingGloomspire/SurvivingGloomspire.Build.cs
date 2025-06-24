// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class SurvivingGloomspire : ModuleRules
{
    public SurvivingGloomspire(ReadOnlyTargetRules target) : base(target)
    {
        // Basic module settings
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        IWYUSupport = IWYUSupport.Full;  // Enforce Include What You Use
        bLegacyPublicIncludePaths = false; // Use explicit public include paths
        
        // Add public include paths
        PublicIncludePaths.AddRange(
        [
            Path.Combine(ModuleDirectory, "Characters"),
            Path.Combine(ModuleDirectory, "Characters/Attributes"),
            Path.Combine(ModuleDirectory, "Characters/Classes"),
            Path.Combine(ModuleDirectory, "Characters/Components"),
            Path.Combine(ModuleDirectory, "Characters/Feats"),
            Path.Combine(ModuleDirectory, "Characters/Skills"),
            Path.Combine(ModuleDirectory, "DataAssets")
        ]);
        
        
        // Public dependencies (modules that expose their header files to us)
        PublicDependencyModuleNames.AddRange(
        [
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "ApplicationCore",
            "Projects",
            "Slate",
            "SlateCore",
            "UMG",
            "EnhancedInput",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        ]);

        // Private dependencies (modules that we use internally)
        PrivateDependencyModuleNames.AddRange(
        [
            // Core modules
            "Slate",
            "SlateCore",
            "UMG",
            "AIModule",
            "NavigationSystem",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "ModularGameplay",
            "CoreUObject",
            "Engine",
            "InputCore",
            "ApplicationCore",
            "Projects",
            "DeveloperSettings",
            "Json",
            "JsonUtilities",
            "HTTP",
            "WebSockets",
            "Networking",
            "Sockets"
        ]);

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // PrivateDependencyModuleNames.Add("OnlineSubsystemNull");
        // PrivateDependencyModuleNames.Add("OnlineSubsystemSteam");
        
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        
        // Uncomment if you are using any plugins
        // PrivateDependencyModuleNames.Add("YourPluginName");
        
        // Enable C++20 features
        CppStandard = CppStandardVersion.Cpp20;
        
        // Enable IWYU with PCH
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Enable exception handling for all modules
        bEnableExceptions = true;
        
        // Enable RTTI
        bUseRTTI = true;
        
        // Configure warning levels
        CppStandard = CppStandardVersion.Cpp20;
        
        // Disable specific warnings
        PrivateDefinitions.Add("HAS_PROGRAMMING_EXTENSIONS=0");
        #if UE_5_6_OR_LATER
        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        #elif UE_5_5_OR_LATER
        UndefinedIdentifierWarningLevel = WarningLevel.Off;
        #else
        bEnableUndefinedIdentifierWarnings = false;
        #endif
        
        // Add source files for the logging system
        PrivateIncludePathModuleNames.AddRange(
            [
                "Settings"
            ]
        );
        
        // Add preprocessor definitions for the logging system
        PublicDefinitions.Add("WITH_LOGGING_SYSTEM=1");
    }
}
