// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Merc_Game : ModuleRules
{
	public Merc_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
