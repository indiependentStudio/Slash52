// 2023, indiependent.Studio. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Slash52EditorTarget : TargetRules
{
	public Slash52EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Slash52" } );
	}
}
