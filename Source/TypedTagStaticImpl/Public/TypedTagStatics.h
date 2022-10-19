// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"

struct FGameplayTagContainer;
struct FGameplayTag;

class TYPEDTAGSTATICIMPL_API FTypedTagStatics
{
public:
	static bool IsBaseOrRemapCategory(const FString& BaseOrRemap, const FGameplayTag& Tag);
	static bool IsBaseOrRemapCategory(const FString& BaseOrRemap, const FGameplayTagContainer& Tags);
};
