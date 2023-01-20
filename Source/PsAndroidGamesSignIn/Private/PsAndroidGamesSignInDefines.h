// Copyright 2015-2023 MY.GAMES. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPsAndroidGamesSignIn, Log, All);

#define PS_FUNC (FString(__FUNCTION__))
#define PS_LINE (FString::FromInt(__LINE__))
#define PS_FUNC_LINE (PS_FUNC + "(" + PS_LINE + ")")
