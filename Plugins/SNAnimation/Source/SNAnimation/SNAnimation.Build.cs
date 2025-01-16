// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SNAnimation : ModuleRules
{
	public SNAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"ApplicationCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"SNPlugin",
				"AIModule",
				"ModularGameplay",
				"GameplayTags",
				// ↓↓Animation制御用
				"AnimGraphRuntime",
				"AnimationWarpingRuntime",
				"StructUtils",
				"Chooser",
				"PoseSearch",
				"MotionWarping",
				// ↑↑ここまで
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
