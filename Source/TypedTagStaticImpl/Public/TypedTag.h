// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "TypedTagLogMacros.h"

//~BEGIN MODIFIED UITag.h (CommonUI)

/** 
 * Implementation utility for typed subclasses of FGameplayTag. 
 * Implemented here instead of directly within the macro to make debugging possible. 
 */
template <typename TagT>
class TTypedTagStaticImpl_Mod
{
	friend TagT;

	static TagT AddNativeTag(const FString& TagBody, const FString& TagDevComment = FString())
	{
		if (!ensure(!TagBody.IsEmpty()))
		{
			return TagT();
		}

		FString TagStr;
		FString RootTagStr = FString::Printf(TEXT("%s."), TagT::GetRootTagStr());
		if (!TagBody.StartsWith(RootTagStr))
		{
			TagStr = RootTagStr + TagBody;
		}
		else
		{
			TagStr = TagBody;
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
			ensureAlwaysMsgf(false, TEXT("Passed unnecessary prefix [%s] when creating a tag of type [%s] with the body [%s]"),
				*RootTagStr, TNameOf<TagT>::GetName(), *TagBody);
#endif
		}

		return UGameplayTagsManager::Get().AddNativeGameplayTag(FName(*TagStr), TagDevComment);
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	/** Intended for console commands/cheats: not for shipping code! */
	static FORCEINLINE TagT FindFromString_DebugOnly(const FString& PartialTagName)
	{
		return UGameplayTagsManager::Get().FindGameplayTagFromPartialString_Slow(PartialTagName);
	}
#endif

	static bool ExportTextItem(const TagT& Tag, FString& ValueStr, int32 PortFlags)
	{
		if (0 == (PortFlags & EPropertyPortFlags::PPF_ExportCpp))
		{
			ValueStr += Tag.GetTagName().ToString();
			return true;
		}
		return false;
	}

	static TagT TryConvert(FGameplayTag VanillaTag, bool bChecked)
	{
		if (VanillaTag.MatchesTag(StaticImpl.RootTag))
		{
			return TagT(VanillaTag);
		}
		else if (VanillaTag.IsValid() && bChecked)
		{
			check(false);
		}
		return TagT();
	}
	
	TTypedTagStaticImpl_Mod()
	{
	//	check(IPluginManager::Get().GetLastCompletedLoadingPhase() < ELoadingPhase::PostEngineInit);
		
		// This is done so UGameplayTagsManager::OnLastChanceToAddNativeTags can safely access this root tag. @see FGameplayTagNativeAdder
		// Modules created at PreDefault or later can safely access this root tag too.
		StaticImpl.RootTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagT::GetRootTagStr());
		
		//IPluginManager::Get().OnLoadingPhaseComplete().AddStatic([](ELoadingPhase::Type LoadingPhase, bool bSuccess)
		//{
		//	if (LoadingPhase >= ELoadingPhase::PreLoadingScreen && LoadingPhase < ELoadingPhase::PostEngineInit && bSuccess)
		//	{
		//	IPluginManager::Get().OnLoadingPhaseComplete().Remove(Handle);
		//	Handle.Reset();
		//
		//	StaticImpl.RootTag = UGameplayTagsManager::Get().AddNativeGameplayTag(TagT::GetRootTagStr());
		//	}
		//}
		//
	}
	
	TagT RootTag;
	static TTypedTagStaticImpl_Mod StaticImpl;
};

template <typename TagT>
TTypedTagStaticImpl_Mod<TagT> TTypedTagStaticImpl_Mod<TagT>::StaticImpl;

// Intended to be the absolute last thing in the definition of a typed tag
#define END_TYPED_TAG_DECL(TagType, TagRoot) \
public:	\
	TagType() {} \
	TagType(FGameplayTag Tag) { /*TTS_ENSURE_TAG_CATEGORY(TagRoot, Tag);*/ TagName = Tag.GetTagName(); } \
	static TagType GetRootTag() { return TTypedTagStaticImpl_Mod<TagType>::StaticImpl.RootTag; }	\
	static TagType TryConvert(FGameplayTag FromTag) { return TTypedTagStaticImpl_Mod<TagType>::TryConvert(FromTag, false); }	\
	static TagType ConvertChecked(FGameplayTag FromTag) { return TTypedTagStaticImpl_Mod<TagType>::TryConvert(FromTag, true); }	\
	static TagType AddNativeTag(const FString& TagBody, const FString& TagDevComment = FString()) { return TTypedTagStaticImpl_Mod<TagType>::AddNativeTag(TagBody, TagDevComment); }	\
	bool ExportTextItem(FString& ValueStr, const TagType& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const	\
	{	\
		return TTypedTagStaticImpl_Mod<TagType>::ExportTextItem(*this, ValueStr, PortFlags);	\
	}	\
protected:	\
	static const TCHAR* GetRootTagStr() { return TagRoot; }	\
	friend class TTypedTagStaticImpl_Mod<TagType>;	\
};	\
Expose_TNameOf(TagType)	\
template<>	\
struct TStructOpsTypeTraits<TagType> : public TStructOpsTypeTraitsBase2<TagType>	\
{	\
	enum	\
	{	\
		WithExportTextItem = true,	\
		WithImportTextItem = true	\
	};
	
//~END MODIFIED UITag.h (CommonUI)

//
// USTRUCT(meta = (Categories = "UX"))
// struct FUXTag : public FGameplayTag
// {
// 	GENERATED_BODY()
// 	END_TYPED_TAG_DECL(FUXTag, TEXT("UX"))
// };
//

//
// FGameplayTagContainer only supports UFUNCTION-level filtering (at the time of writing).
//
// In C++20, we can pass string literals as template parameters...
// TGameplayTag<"TagRoot"> ?????????????????? 
// TSubtagOf<"TagRoot"> ?????????????????????
// TGameplayTagContainer<"TagRoot"> ?????????
//
// https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
//
