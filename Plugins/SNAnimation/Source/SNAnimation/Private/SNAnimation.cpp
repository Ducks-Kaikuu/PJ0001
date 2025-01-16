// Copyright Epic Games, Inc. All Rights Reserved.

#include "SNAnimation.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FSNAnimationModule"

void FSNAnimationModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	Manager.AddNativeGameplayTag(TEXT("Abilities.ContextualAnim"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Abilities.MoveTo"), TEXT(""));
	
	Manager.AddNativeGameplayTag(TEXT("Foley.Event"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Handplant"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Jump"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Land"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Run"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.RunBackwds"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.RunStrafe"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Scuff"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.ScuffPivot"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.ScuffWall"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Tumble"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.Walk"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Foley.Event.WalkBackwds"), TEXT(""));
	
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNJump"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNLook"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNLook_Gamepad"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNMove"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNMove_WorldSpace"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNWalk"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNSprint"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNStrafe"), TEXT(""));
	Manager.AddNativeGameplayTag(TEXT("Input.Normal.SNAim"), TEXT(""));
}

void FSNAnimationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSNAnimationModule, SNAnimation)