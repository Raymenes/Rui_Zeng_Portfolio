// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TetrisClimber : ModuleRules
{
	public TetrisClimber(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
