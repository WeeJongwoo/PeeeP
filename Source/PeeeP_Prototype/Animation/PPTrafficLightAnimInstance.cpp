// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPTrafficLightAnimInstance.h"
#include "InteractionObject/PPTrafficLightSkeletal.h"
#include "InteractionObject/ETrafficLight.h"

UPPTrafficLightAnimInstance::UPPTrafficLightAnimInstance()
{

}

void UPPTrafficLightAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TrafficLight = Cast<APPTrafficLightSkeletal>(GetOwningActor());
	if (TrafficLight)
	{
		switch (TrafficLight->GetCurrentTrafficLightColor())
		{
		case ETrafficLightColor::TC_RED:
		case ETrafficLightColor::TC_YELLOW:
			bIsRedLight = true;
			break;
		case ETrafficLightColor::TC_GREEN:
			bIsRedLight = false;
			break;
		default:
			break;
		}
	}
}

void UPPTrafficLightAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (TrafficLight)
	{
		switch (TrafficLight->GetCurrentTrafficLightColor())
		{
		case ETrafficLightColor::TC_RED:
		case ETrafficLightColor::TC_YELLOW:
			bIsRedLight = true;
			break;
		case ETrafficLightColor::TC_GREEN:
			bIsRedLight = false;
			break;
		default:
			break;
		}
	}
}
