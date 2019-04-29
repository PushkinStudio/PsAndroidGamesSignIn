// Copyright 2015-2019 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PsAndroidGamesSignIn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPsAndroidGamesSignIn, Log, All);
#define PS_FUNC (FString(__FUNCTION__))
#define PS_LINE (FString::FromInt(__LINE__))
#define PS_FUNC_LINE (PS_FUNC + "(" + PS_LINE + ")")

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPsAndroidGamesSignInDelegate, bool, bSuccess, FString, ServerAuthCode);

UCLASS()
class UPsAndroidGamesSignIn : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Sign in silently, without presenting UI */
	UFUNCTION(BlueprintCallable)
	static void SignInSilently(const FPsAndroidGamesSignInDelegate& CallbackDelegate);

	/** Sign in interactively, with UI if needed */
	UFUNCTION(BlueprintCallable)
	static void SignInInteractively(const FPsAndroidGamesSignInDelegate& CallbackDelegate);

	/** Callback delegate instance */
	static FPsAndroidGamesSignInDelegate Delegate;
};
