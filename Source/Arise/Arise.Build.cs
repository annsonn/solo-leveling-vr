using UnrealBuildTool;

public class Arise : ModuleRules
{
	public Arise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});

		// GAS modules (GameplayAbilities, GameplayTags, GameplayTasks) added in Phase 4.
	}
}
