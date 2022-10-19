// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

using UnrealBuildTool;

public class TypedTagStaticImpl : ModuleRules
{
	public TypedTagStaticImpl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayTags",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
			}
		);
	}
}
