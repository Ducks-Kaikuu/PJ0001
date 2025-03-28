// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PJ0000 : ModuleRules
{
	public PJ0000(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags", "AIModule", "Niagara", "NiagaraCore" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// ↓↓Animation制御用
			"AnimGraphRuntime",
			"AnimationWarpingRuntime",
			"StructUtils",
			"Chooser",
			"PoseSearch",
			"MotionWarping",
			// ↑↑ここまで
			"StateTreeModule",
			"EngineSettings",
			"UMG",
			"Slate",
			"SlateCore",
			"FieldSystemEngine",
			"GeometryCollectionEngine"
		});
		
		PrivateIncludePaths.AddRange(new string[]
		{
			"PJ0000",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
