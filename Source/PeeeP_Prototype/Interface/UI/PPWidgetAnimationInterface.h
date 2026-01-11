// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PPWidgetAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPPWidgetAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PEEEP_PROTOTYPE_API IPPWidgetAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PlayWidgetAnimationIn() = 0;
	virtual void PlayWidgetAnimation() = 0;
	virtual void PlayWidgetAnimationOut() = 0;
};
