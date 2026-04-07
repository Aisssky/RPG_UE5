// Copyright Aisssky

using UnrealBuildTool;
using System.Collections.Generic;

public class ChaPandaEditorTarget : TargetRules
{
	public ChaPandaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ChaPanda" } );
	}
}
