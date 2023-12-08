// Fill out your copyright notice in the Description page of Project Settings.

using System;
using UnrealBuildTool;

public class ProjectGame : ModuleRules
{
    public ProjectGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Core
        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput"
        });

        // UI
        PrivateDependencyModuleNames.AddRange(new string[] {
            "UMG",
            "Slate",
            "SlateCore"
        });

        // Gameplay Ability System
        PublicDependencyModuleNames.AddRange(new string[] { 
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "ProjectGame/Public/Player",
            "ProjectGame/Public/Components",
            "ProjectGame/Public/Dev",
            "ProjectGame/Public/Weapon",
            "ProjectGame/Public/UI",
             "ProjectGame/Public/Animation",
             "ProjectGame/Public/Environment",
             "ProjectGame/Public/Items",
             "ProjectGame/Public/Abilities"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
