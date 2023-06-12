// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyBohemianLife : ModuleRules
{
	public MyBohemianLife(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara" });
	}
}
