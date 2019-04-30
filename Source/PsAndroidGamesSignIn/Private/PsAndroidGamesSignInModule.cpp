// Copyright 2015-2019 Mail.Ru Group. All Rights Reserved.

#include "PsAndroidGamesSignInModule.h"

#include "PsAndroidGamesSignInDefines.h"
#include "PsAndroidGamesSignInSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "UObject/Package.h"

DEFINE_LOG_CATEGORY(LogPsAndroidGamesSignIn);

#define LOCTEXT_NAMESPACE "FPsAndroidGamesSignInModule"

void FPsAndroidGamesSignInModule::StartupModule()
{
	Settings = NewObject<UPsAndroidGamesSignInSettings>(GetTransientPackage(), "PsAndroidGamesSignInSettings", RF_Standalone);
	Settings->AddToRoot();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "PsAndroidGamesSignIn",
			LOCTEXT("RuntimeSettingsName", "PsAndroidGamesSignIn"),
			LOCTEXT("RuntimeSettingsDescription", "Configure PsAndroidGamesSignIn"),
			Settings);
	}
}

void FPsAndroidGamesSignInModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "PsAndroidGamesSignIn");
	}

	if (!GExitPurge)
	{
		Settings->RemoveFromRoot();
	}
	else
	{
		Settings = nullptr;
	}
}

UPsAndroidGamesSignInSettings* FPsAndroidGamesSignInModule::GetSettings() const
{
	check(Settings);
	return Settings;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPsAndroidGamesSignInModule, PsAndroidGamesSignIn)
