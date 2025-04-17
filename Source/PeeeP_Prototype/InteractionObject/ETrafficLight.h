// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class ETrafficLightType : uint8
{
	TL_NONE,
	TL_TWO,
	TL_THREE,
	TL_THREE_WITH_BAR
};

UENUM(BlueprintType)
enum class ETrafficLightColor : uint8
{
	TC_GREEN,
	TC_YELLOW,
	TC_RED,
	TC_OFF
};