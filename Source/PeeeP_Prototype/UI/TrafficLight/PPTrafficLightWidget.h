// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "PPTrafficLightWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPTrafficLightWidget : public UPPCustomWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class APPTrafficLightManager> TrafficLightManager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Background;
	UPROPERTY()
	TObjectPtr<class UImage> TrafficLightsImage;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> TrafficLight01_Color;
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> TrafficLight02_Color;
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> TrafficLight03_Color;



public:
	void SetTrafficLightColorVisible(bool bFlag);
};
