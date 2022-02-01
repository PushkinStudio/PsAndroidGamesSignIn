// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PsAndroidGamesSignIn.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPsAndroidGamesSignInDelegate, bool, bSuccess, FString, ServerAuthCode);

DECLARE_DELEGATE_TwoParams(FPsAndroidGamesSignInDelegateStatic, bool, FString);

UCLASS()
class UPsAndroidGamesSignIn : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** Sign in silently, without presenting UI (BP version) */
	UFUNCTION(BlueprintCallable)
	static void SignInSilently(const FPsAndroidGamesSignInDelegate& CallbackDelegate);

	/** Sign in silently, without presenting UI */
	static void SignInSilently(const FPsAndroidGamesSignInDelegateStatic& CallbackDelegate);

	/** Sign in interactively, with UI if needed (BP version) */
	UFUNCTION(BlueprintCallable)
	static void SignInInteractively(const FPsAndroidGamesSignInDelegate& CallbackDelegate);

	/** Sign in interactively, with UI if needed */
	static void SignInInteractively(const FPsAndroidGamesSignInDelegateStatic& CallbackDelegate);

	/** Callback dynamic delegate instance */
	static FPsAndroidGamesSignInDelegate Delegate;

	/** Callback static delegate instance */
	static FPsAndroidGamesSignInDelegateStatic StaticDelegate;

private:
	/** Sign-in implementation */
	static void SignInSilentlyImpl();

	/** Sign-in implementation */
	static void SignInInteractivelyImpl();
};
