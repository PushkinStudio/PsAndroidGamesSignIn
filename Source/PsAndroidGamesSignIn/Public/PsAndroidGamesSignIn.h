// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

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
	/** Login (BP version) */
	UFUNCTION(BlueprintCallable)
	static void Login(const FPsAndroidGamesSignInDelegate& CallbackDelegate);

	/** Login */
	static void Login(const FPsAndroidGamesSignInDelegateStatic& CallbackDelegate);

public:
	/** Callback dynamic delegate instance */
	static FPsAndroidGamesSignInDelegate Delegate;

	/** Callback static delegate instance */
	static FPsAndroidGamesSignInDelegateStatic StaticDelegate;

private:
	/** Login implementation */
	static void LoginImpl();
};
