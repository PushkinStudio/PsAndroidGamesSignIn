// Copyright 2015-2019 Mail.Ru Group. All Rights Reserved.

#include "PsAndroidGamesSignIn.h"

#include "PsAndroidGamesSignInDefines.h"

#if PLATFORM_ANDROID
#include "Async.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include <android_native_app_glue.h>
#endif // PLATFORM_ANDROID

FPsAndroidGamesSignInDelegate UPsAndroidGamesSignIn::Delegate;

UPsAndroidGamesSignIn::UPsAndroidGamesSignIn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPsAndroidGamesSignIn::SignInSilently(const FPsAndroidGamesSignInDelegate& CallbackDelegate)
{
#if PLATFORM_ANDROID
	UPsAndroidGamesSignIn::Delegate = CallbackDelegate;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv(true);
	if (Env)
	{
		jmethodID methodId = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PsGoogleLogin_SignInSilently", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, methodId);
	}
	else
	{
		UE_LOG(LogPsAndroidGamesSignIn, Error, TEXT("%s: invalid JNIEnv"), *PS_FUNC_LINE);
	}
#endif // PLATFORM_ANDROID
}

void UPsAndroidGamesSignIn::SignInInteractively(const FPsAndroidGamesSignInDelegate& CallbackDelegate)
{
#if PLATFORM_ANDROID
	UPsAndroidGamesSignIn::Delegate = CallbackDelegate;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv(true);
	if (Env)
	{
		jmethodID methodId = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PsGoogleLogin_SignInInteractively", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, methodId);
	}
	else
	{
		UE_LOG(LogPsAndroidGamesSignIn, Error, TEXT("%s: invalid JNIEnv"), *PS_FUNC_LINE);
	}
#endif // PLATFORM_ANDROID
}

#if PLATFORM_ANDROID

JNI_METHOD void Java_com_pushkinstudio_PsAndroidGamesSignIn_PsGoogleLogin_nativeGoogleLoginCompleted(JNIEnv* jenv, jobject thiz, jboolean Success, jstring ServerAuthCode)
{
	FString AccessToken;
	if (Success)
	{
		const char* charsToken = jenv->GetStringUTFChars(ServerAuthCode, 0);
		AccessToken = FString(UTF8_TO_TCHAR(charsToken));
		jenv->ReleaseStringUTFChars(ServerAuthCode, charsToken);
	}

	AsyncTask(ENamedThreads::GameThread, [Success, AccessToken]() {
		UE_LOG(LogPsAndroidGamesSignIn, Warning, TEXT("%s: GoogleLoginCompleted Success: %d ServerAuthCode: \"%s\""), *PS_FUNC_LINE, Success, *AccessToken);
		UPsAndroidGamesSignIn::Delegate.ExecuteIfBound(Success, AccessToken);
		UPsAndroidGamesSignIn::Delegate.Clear();
	});
}

#endif // PLATFORM_ANDROID
