// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TrafficLight/PPTrafficLightThree.h"
#include "Components/Image.h"

void UPPTrafficLightThree::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(TrafficLight))
	{
		if (IsValid(TrafficLightColor_Red))
		{
			TrafficLightColor_Red->SetVisibility(ESlateVisibility::Hidden);
		}
		if (IsValid(TrafficLightColor_Yellow))
		{
			TrafficLightColor_Yellow->SetVisibility(ESlateVisibility::Hidden);
		}	
		if (IsValid(TrafficLightColor_Green))
		{
			TrafficLightColor_Green->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPPTrafficLightThree::SetTrafficLightColorVisible(ETrafficLightColor Color)
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
	case ETrafficLightColor::TC_YELLOW:
	{
		TrafficLightColor_Yellow->SetVisibility(ESlateVisibility::Visible);
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

void UPPTrafficLightThree::SetAllTrafficLightColorOff()
{
	TrafficLightColor_Red->SetVisibility(ESlateVisibility::Hidden);
	TrafficLightColor_Yellow->SetVisibility(ESlateVisibility::Hidden);
	TrafficLightColor_Green->SetVisibility(ESlateVisibility::Hidden);
}
