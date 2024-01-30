// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Gamahero : ModuleRules
{
	public Project_Gamahero(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
