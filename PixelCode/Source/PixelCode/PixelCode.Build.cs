// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PixelCode : ModuleRules
{
	public PixelCode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
