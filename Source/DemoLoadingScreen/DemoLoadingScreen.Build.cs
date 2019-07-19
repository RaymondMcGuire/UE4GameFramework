// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class DemoLoadingScreen : ModuleRules
{
    public DemoLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "Public/DemoLoadingScreen.h";

        PCHUsage = PCHUsageMode.UseSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
                "MoviePlayer"
            }
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);

        PublicIncludePaths.AddRange(new string[] { "DemoLoadingScreen/Public" });
        PrivateIncludePaths.AddRange(new string[] { "DemoLoadingScreen/Private" });
    }
}
