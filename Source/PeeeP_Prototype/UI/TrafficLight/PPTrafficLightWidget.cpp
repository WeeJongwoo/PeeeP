// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TrafficLight/PPTrafficLightWidget.h"
#include "Components/Image.h"
#include "InteractionObject/PPTrafficLightManager.h"

void UPPTrafficLightWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Background)
	{
		Background->SetVisibility(ESlateVisibility::Visible);
	}

}

void UPPTrafficLightWidget::SetTrafficLightColorVisible(bool bFlag)
{
}
