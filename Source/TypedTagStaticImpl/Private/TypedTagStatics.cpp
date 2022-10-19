// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#include "TypedTagStatics.h"
#include "GameplayTagsSettings.h"

bool FTypedTagStatics::IsBaseOrRemapCategory(const FString& BaseOrRemap, const FGameplayTag& Tag)
{
	if (BaseOrRemap.IsEmpty() || !Tag.IsValid())
	{
		// Always return true when the provided arguments are empty or incomplete
		return true;
	}
	
	const UGameplayTagsSettings* GameplayTagsSettings = GetDefault<UGameplayTagsSettings>();
	check(GameplayTagsSettings);
	
	for (const FGameplayTagCategoryRemap& ExistingRemap : GameplayTagsSettings->CategoryRemapping)
	{
		if (ExistingRemap.BaseCategory.Contains(BaseOrRemap))
		{
			if (Tag.ToString().Contains(ExistingRemap.BaseCategory))
			{
				return true;
			}
		}
		else
		{
			for (const FString& RemapCategory : ExistingRemap.RemapCategories)
			{
				if (RemapCategory.Contains(BaseOrRemap))
				{
					//@TODO This string comparison seems heavy. Doing a shared pointer comparison straight from UGameplayTagsManager seems like the better alternative.
					if (Tag.ToString().Contains(RemapCategory))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool FTypedTagStatics::IsBaseOrRemapCategory(const FString& BaseOrRemap, const FGameplayTagContainer& Tags)
{
	if (BaseOrRemap.IsEmpty() || Tags.IsEmpty())
	{
		// Always return true when the provided arguments are empty or incomplete.
		return true;
	}
	
	const UGameplayTagsSettings* GameplayTagsSettings = GetDefault<UGameplayTagsSettings>();
	check(GameplayTagsSettings);
	
	for (const FGameplayTagCategoryRemap& ExistingRemap : GameplayTagsSettings->CategoryRemapping)
	{
		if (ExistingRemap.BaseCategory.Contains(BaseOrRemap))
		{
			for (const FGameplayTag& Tag : Tags)
			{
				//@TODO This string comparison seems heavy. Doing a shared pointer comparison straight from UGameplayTagsManager seems like the better alternative.
				if (Tag.ToString().Contains(ExistingRemap.BaseCategory))
				{
					return true;
				}
			}
		}
		else
		{
			for (const FString& RemapCategory : ExistingRemap.RemapCategories)
			{
				if (RemapCategory.Contains(BaseOrRemap))
				{
					for (const FGameplayTag& Tag : Tags)
					{
						if (Tag.ToString().Contains(RemapCategory))
						{
							return true;
						}
					}
				}
			}
		}
	}
	
	return false;
}

