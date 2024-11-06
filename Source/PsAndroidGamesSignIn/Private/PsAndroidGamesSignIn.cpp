// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#include "PsAndroidGamesSignIn.h"

#include "PsAndroidGamesSignInSettings.h"

#include "Async/TaskGraphInterfaces.h"

#if PLATFORM_ANDROID
#include "PsAndroidGamesSignInDefines.h"

#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Async/Async.h"
#include <android_native_app_glue.h>
#endif // PLATFORM_ANDROID

FPsAndroidGamesSignInDelegate UPsAndroidGamesSignIn::Delegate;
FPsAndroidGamesSignInDelegateStatic UPsAndroidGamesSignIn::StaticDelegate;

UPsAndroidGamesSignIn::UPsAndroidGamesSignIn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPsAndroidGamesSignIn::Login(const FPsAndroidGamesSignInDelegate& CallbackDelegate)
{
#if PLATFORM_ANDROID
	Delegate = CallbackDelegate;
	StaticDelegate = FPsAndroidGamesSignInDelegateStatic();
	LoginImpl();
#endif // PLATFORM_ANDROID
}

void UPsAndroidGamesSignIn::Login(const FPsAndroidGamesSignInDelegateStatic& CallbackDelegate)
{
#if PLATFORM_ANDROID
	Delegate = FPsAndroidGamesSignInDelegate();
	StaticDelegate = CallbackDelegate;
	LoginImpl();
#endif // PLATFORM_ANDROID
}

void UPsAndroidGamesSignIn::LoginImpl()
{
#if PLATFORM_ANDROID
	JNIEnv* Env = FAndroidApplication::GetJavaEnv(true);
	if (Env)
	{
		jclass ClassID = FAndroidApplication::FindJavaClass("com/pushkinstudio/PsAndroidGamesSignIn/PsGoogleLogin");
		jmethodID LoginMethodId = FJavaWrapper::FindStaticMethod(Env, ClassID, "Login", "(Landroid/app/Activity;Ljava/lang/String;)V", false);
		if (LoginMethodId)
		{
			const auto Settings = GetDefault<UPsAndroidGamesSignInSettings>();
			FScopedJavaObject<jstring> ServerClientId = FJavaHelper::ToJavaString(Env, Settings->ServerClientId);
			FJavaWrapper::CallStaticVoidMethod(Env, ClassID, LoginMethodId, AndroidJavaEnv::GetGameActivityThis(), *ServerClientId);
		}
		else
		{
			UE_LOG(LogPsAndroidGamesSignIn, Error, TEXT("%s: invalid LoginMethodId"), *PS_FUNC_LINE);
		}
	}
	else
	{
		UE_LOG(LogPsAndroidGamesSignIn, Error, TEXT("%s: invalid JNIEnv"), *PS_FUNC_LINE);
	}
#endif // PLATFORM_ANDROID
}

#if PLATFORM_ANDROID
JNI_METHOD void Java_com_pushkinstudio_PsAndroidGamesSignIn_PsGoogleLogin_nativeGoogleLoginCompleted(JNIEnv* jenv, jclass /*clazz*/, jboolean Success, jstring ServerAuthCode)
{
	if (!FTaskGraphInterface::IsRunning())
	{
		UE_LOG(LogPsAndroidGamesSignIn, Warning, TEXT("%s: FTaskGraphInterface isn't running"), *PS_FUNC_LINE);
		return;
	}

	FString AccessToken;
	if (Success)
	{
		const char* charsToken = jenv->GetStringUTFChars(ServerAuthCode, 0);
		AccessToken = FString(UTF8_TO_TCHAR(charsToken));
		jenv->ReleaseStringUTFChars(ServerAuthCode, charsToken);
	}

	AsyncTask(ENamedThreads::GameThread, [Success, AccessToken]() {
		UE_LOG(LogPsAndroidGamesSignIn, Display, TEXT("%s: GoogleLoginCompleted Success: %d ServerAuthCode: \"%s\""), *PS_FUNC_LINE, Success, *AccessToken);
		UPsAndroidGamesSignIn::Delegate.ExecuteIfBound(Success, AccessToken);
		UPsAndroidGamesSignIn::StaticDelegate.ExecuteIfBound(Success, AccessToken);
	});
}
#endif // PLATFORM_ANDROID
