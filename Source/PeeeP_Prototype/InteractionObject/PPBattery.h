// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionObject/PPGrabableObject.h"
#include "GameplayTagContainer.h"
#include "PPBattery.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPBattery : public APPGrabableObject
{
	GENERATED_BODY()
	
protected:
	APPBattery();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer MyTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTag BatteryTag;
};
