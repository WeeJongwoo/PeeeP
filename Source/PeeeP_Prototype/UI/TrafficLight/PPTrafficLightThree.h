// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "InteractionObject/ETrafficLight.h"
#include "PPTrafficLightThree.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPTrafficLightThree : public UPPCustomWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TrafficLight;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TrafficLightColor_Red;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TrafficLightColor_Yellow;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TrafficLightColor_Green;

public:
	void SetTrafficLightColorVisible(ETrafficLightColor Color);

	void SetAllTrafficLightColorOff();
};
