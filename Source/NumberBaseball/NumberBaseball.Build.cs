// NumberBaseball.Build.cs

using UnrealBuildTool;

public class NumberBaseball : ModuleRules
{
	public NumberBaseball(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Initial Dependencies
			"Core", "CoreUObject", "Engine", "InputCore",

			// UI
			"UMG", "Slate", "SlateCore",
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"NumberBaseball",
		});

	}
}
