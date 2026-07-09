using UnrealBuildTool;
using System.Collections.Generic;

public class SoloLevelingVRTarget : TargetRules
{
	public SoloLevelingVRTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Arise");
	}
}
