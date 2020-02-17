// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class UPsAndroidGamesSignInSettings;

class FPsAndroidGamesSignInModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;

	/** IModuleInterface implementation */
	virtual void ShutdownModule() override;

	static inline FPsAndroidGamesSignInModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FPsAndroidGamesSignInModule>("PsAndroidGamesSignIn");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("PsAndroidGamesSignIn");
	}

	/** Get Settings instance */
	UPsAndroidGamesSignInSettings* GetSettings() const;

private:
	/** Settings instance */
	UPsAndroidGamesSignInSettings* Settings;
};
