// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

#pragma once

#include "CoreMinimal.h"
#include "TypedTagStatics.h"
#include "VisualLogger/VisualLogger.h"

#define TTS_MSGF_FORMAT_SINGLETAG(TagType, TagCategory)	TEXT("Expected \"" #TagType "\" to be mapped to \"" TagCategory "\". Mappings live inside GameplayTagsSettings under Edit > Project Settings > GameplayTags")
#define TTS_MSGF_FORMAT_TAGCONTAINER(TagType, TagCategory) TEXT("Expected \"" #TagType "\" to only have tags mapped to \"" TagCategory "\". Mappings live inside GameplayTagsSettings under Edit > Project Settings > GameplayTags")

#if ENABLE_VISUAL_LOG

// @Lucas- We should also accept types with operator FGameplayTag& / FGameplayTagContainer&, or whether TIsConstructible<> etc, instead of only accepting FGameplayTag or FGameplayTagContainer.

/**
 * If VisualLogger is recording, make sure TagType belongs under TagCategory as Base- or Remap Category.
 * @see GameplayTagsSettings::CategoryRemapping
 *
 * @parm LogOwner		UE_VLOG LogOwner
 * @parm CategoryName	UE_VLOG CategoryName
 * @parm Verbosity		UE_VLOG Verbosity
 * @parm TagCategory	The tag base/remap category as TEXT("")
 * @parm TagType		A FGameplayTag or FGameplayTagContainer.
 */
#define TTS_VLOG_TAG_CATEGORY(LogOwner, CategoryName, Verbosity, TagCategory, TagType) \
static_assert(TIsArrayOrRefOfType<decltype(TagCategory), TCHAR>::Value, "TagCategory must be a TCHAR array."); \
static_assert(TOr<TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTag>, \
                  TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTagContainer>>::Value, \
                  "The TagType macro argument should be a FGameplayTag or FGameplayTagContainer"); \
\
if constexpr (TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTagContainer>::Value) \
{ \
	UE_CVLOG(FTypedTagStatics::IsBaseOrRemapCategory(TagCategory, TagType), LogOwner, CategoryName, Verbosity, TTS_MSGF_FORMAT_TAGCONTAINER(TagType, TagCategory)); \
} \
else if constexpr (TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTag>::Value) \
{ \
	UE_CVLOG(FTypedTagStatics::IsBaseOrRemapCategory(TagCategory, TagType), LogOwner, CategoryName, Verbosity, TTS_MSGF_FORMAT_SINGLETAG(TagType, TagCategory)); \
}

#else
#define TTS_VLOG_TAG_CATEGORY(LogOwner, CategoryName, Verbosity, TagCategory, TagType)
#endif // ENABLE_VISUAL_LOG

/**
 * Assert that TagType belongs under TagCategory as Base- or Remap Category.
 * @see GameplayTagsSettings::CategoryRemapping
 * 
 * @parm MsgfAssert		Assertion macro to use
 * @parm TagCategory	The tag base/remap category as TEXT("")
 * @parm TagType		A FGameplayTag or FGameplayTagContainer.
 */
#define TTS_MSGFASSERT_TAG_CATEGORY(MsgfAssert, TagCategory, TagType) \
static_assert(TIsArrayOrRefOfType<decltype(TagCategory), TCHAR>::Value, "TagCategory must be a TCHAR array."); \
static_assert(TOr<TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTag>, \
				  TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTagContainer>>::Value, \
                  "The TagType macro argument should be a FGameplayTag or FGameplayTagContainer"); \
 \
if constexpr (TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTagContainer>::Value) \
{ \
	MsgfAssert(FTypedTagStatics::IsBaseOrRemapCategory(TagCategory, TagType), TTS_MSGF_FORMAT_TAGCONTAINER(TagType, TagCategory)); \
} \
else if constexpr (TIsSame<TDecay<decltype(TagType)>::Type, FGameplayTag>::Value) \
{ \
	MsgfAssert(FTypedTagStatics::IsBaseOrRemapCategory(TagCategory, TagType), TTS_MSGF_FORMAT_SINGLETAG(TagType, TagContainer)); \
}

#if DO_GUARD_SLOW
	/**
	 * checkSlow whether TagType belongs under TagCategory as Base- or Remap Category. @see GameplayTagsSettings::CategoryRemapping
	 * Warning: This might be too strict. Often times, the usage of TTS_VLOG_TAG_CATEGORY supersedes this.
	 */
	#define TTS_CHECKSLOW_TAG_CATEGORY(TagCategory, TagType) TTS_MSGFASSERT_TAG_CATEGORY(checkfSlow, TagCategory, TagType)
#else // DO_GUARD_SLOW
	#define TTS_CHECKSLOW_TAG_CATEGORY(TagCategory, TagType)
#endif // DO_GUARD_SLOW

#if DO_ENSURE
	/**
	 * ensure that TagType belongs under TagCategory as Base- or Remap Category. @see GameplayTagsSettings::CategoryRemapping
	 */
	#define TTS_ENSURE_TAG_CATEGORY(TagCategory, TagType) TTS_MSGFASSERT_TAG_CATEGORY(ensureMsgf, TagCategory, TagType)
#else // DO_ENSURE
	#define TTS_ENSURE_TAG_CATEGORY(TagCategory, TagType)
#endif // DO_ENSURE
