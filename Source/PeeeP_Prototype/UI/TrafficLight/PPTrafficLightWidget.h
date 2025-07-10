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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> TrafficLightsBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget")
	TSubclassOf<class UPPTrafficLightThree> TrafficLightThreeWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget")
	TSubclassOf<class UPPTrafficLightTwo> TrafficLightTwoWidgetClass;

	TMap<class APPTrafficLightBase*, class UPPCustomWidget*> TrafficLightsWithWidget;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> BackgroundAppearAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> BackgroundDisappearAnim;

	FTimerHandle ExitAnimationTimerHandle;

	void OnExitAnimationFinished();

public:
	void RefreshTrafficLightColor();

	void DisableTrafficLightWidget();
};
