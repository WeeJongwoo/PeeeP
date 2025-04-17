// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPBattery.h"

APPBattery::APPBattery()
{
	//Super::APPGrabableObject();

	MyTags.AddTag(FGameplayTag::RequestGameplayTag(FName("TrafficLight.Battery")));
	BatteryTag = FGameplayTag::RequestGameplayTag(FName("TrafficLight.Battery"));
}
