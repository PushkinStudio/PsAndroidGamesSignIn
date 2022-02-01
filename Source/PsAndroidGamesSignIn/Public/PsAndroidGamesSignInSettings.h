// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsAndroidGamesSignInSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UPsAndroidGamesSignInSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/** Server Client Id */
	UPROPERTY(Config, EditAnywhere)
	FString ServerClientId;
};
