// Copyright Aisssky

using UnrealBuildTool;
using System.Collections.Generic;

public class ChaPandaTarget : TargetRules
{
	public ChaPandaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		bCompressDebugFile = false; // LLVM 官方版不带 zlib，关闭调试信息压缩

		ExtraModuleNames.AddRange( new string[] { "ChaPanda" } );
	}
}
