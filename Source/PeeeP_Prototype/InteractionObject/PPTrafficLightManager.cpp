// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLightManager.h"
#include "InteractionObject/PPTrafficLight.h"
#include "InteractionObject/Electric/PPTrafficLightController.h"

// Sets default values
APPTrafficLightManager::APPTrafficLightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPTrafficLightManager::BeginPlay()
{
	Super::BeginPlay();

	// TrafficLightManagerActor가 관리하는 신호등 제어기 배열을 가져옴
	for (APPTrafficLightController* TrafficLightController : TrafficLightControllers)
	{
		if (IsValid(TrafficLightController))
		{
			TrafficLightController->OnTrafficLightColorChangedDelegate.BindUObject(this, &APPTrafficLightManager::CheckTrafficLightsColor);
		}
	}
}

// Called every frame
void APPTrafficLightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPTrafficLightManager::CheckTrafficLightsColor(ETrafficLightColor Color)
{
	// TrafficLights안에 있는 신호등들의 색깔을 모두 검사
	for (APPTrafficLight* TrafficLight : TrafficLights)
	{
		if (IsValid(TrafficLight))
		{
			ETrafficLightColor CurrentTrafficLightColor = TrafficLight->GetCurrentTrafficLightColor();
			// 신호등 중 하나라도 지정한 Color와 다르면 retrun
			if (CurrentTrafficLightColor != Color)
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	// 모든 조건을 만족하면 StartEvent() 실행
	StartEvent();
}

void APPTrafficLightManager::StartEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("All Traffic Lights are Matched!"));
}

