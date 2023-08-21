// 2023, indiependent.studio. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Slash52Target : TargetRules
{
	public Slash52Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Slash52" } );
	}
}
