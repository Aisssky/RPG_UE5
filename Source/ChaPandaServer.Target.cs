using UnrealBuildTool;
using System.Collections.Generic;

public class ChaPandaServerTarget : TargetRules
{
    public ChaPandaServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "ChaPanda" });
    }
}