// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SNPlugin : ModuleRules
{
	public SNPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		// OSSV1を使わない
		bool bUseOnlineSubsystemV1 = false;
		//bool bUseOnlineSubsystemV1 = true;
		
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
				"CoreUObject",
				"CoreOnline",
				"ApplicationCore",
				"NetCore",
				"Engine",
				"ChaosVehicles",
				"GameplayTags",
				"GameplayTasks",
				"StateTreeModule",
				"GameplayStateTreeModule",
				"UMG", 
				"EnhancedInput", 
				"GameplayAbilities",
				"OpenSSL",
				// ... add other public dependencies that you statically link with here ...
			}
		);
		
		PublicDefinitions.Add("COMMONUSER_OSSV1=" + (bUseOnlineSubsystemV1 ? "1" : "0"));
		
		if (bUseOnlineSubsystemV1)
		{
			PublicDependencyModuleNames.Add("OnlineSubsystem");
		}
		else
		{
			PublicDependencyModuleNames.Add("OnlineServicesInterface");
			PublicDependencyModuleNames.Add("OnlineServicesEOSGS");
		}

		if (Target.Type == TargetType.Editor)
		{
			PublicDefinitions.Add("SN_DEBUG_SAVE_DATA");
		}
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"AIModule",
				"EnhancedInput",
				"ModularGameplay",
				"OnlineSubsystemUtils",
				"GameplayAbilities",
				"Chooser",
				"PoseSearch",
				"AnimGraphRuntime",
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
