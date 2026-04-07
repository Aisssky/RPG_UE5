// Copyright Aisssky

using UnrealBuildTool;
using System.Collections.Generic;

public class ChaPandaTarget : TargetRules
{
	public ChaPandaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ChaPanda" } );
	}
}
