// Copyright 2015-2021 Mail.Ru Group. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class PsAndroidGamesSignIn : ModuleRules
{
    public PsAndroidGamesSignIn(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
			}
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
			}
            );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "PsAndroidGamesSignIn_UPL_Android.xml"));

            PublicDependencyModuleNames.AddRange(new string[] { "Launch" });
        }
    }
}
