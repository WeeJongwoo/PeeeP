// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TrafficLight/PPTrafficLightTwo.h"
#include "Components/Image.h"

void UPPTrafficLightTwo::NativeConstruct()
{
	Super::NativeConstruct();

	if (TrafficLight)
	{
		if (IsValid(TrafficLightColor_Red))
		{
			TrafficLightColor_Red->SetVisibility(ESlateVisibility::Hidden);
		}
		if (IsValid(TrafficLightColor_Green))
		{
			TrafficLightColor_Green->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPPTrafficLightTwo::SetTrafficLightColorVisible(ETrafficLightColor Color)
{
	SetAllTrafficLightColorOff();

	switch (Color)
	{
	case ETrafficLightColor::TC_OFF:
		break;
	case ETrafficLightColor::TC_RED:
	{
		TrafficLightColor_Red->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case ETrafficLightColor::TC_GREEN:
	{
		TrafficLightColor_Green->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	default:
		break;
	}
}

void UPPTrafficLightTwo::SetAllTrafficLightColorOff()
{
	TrafficLightColor_Red->SetVisibility(ESlateVisibility::Hidden);
	TrafficLightColor_Green->SetVisibility(ESlateVisibility::Hidden);
}
