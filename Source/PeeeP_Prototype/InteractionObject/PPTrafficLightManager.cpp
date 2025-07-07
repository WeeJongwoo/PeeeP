// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLightManager.h"
#include "InteractionObject/PPTrafficLightBase.h"
#include "InteractionObject/Electric/PPTrafficLightController.h"
#include "Interface/PPTrafficLightEventInterface.h"
#include "Components/AudioComponent.h"

// Sets default values
APPTrafficLightManager::APPTrafficLightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	SetRootComponent(AudioComponent);

}

// Called when the game starts or when spawned
void APPTrafficLightManager::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent)
	{
		if (ClearSound)
		{
			AudioComponent->SetSound(ClearSound);
		}
	}

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
	// Traffic Light UI
	if (RefreshTrafficLightsColorDelegate.IsBound())
	{
		RefreshTrafficLightsColorDelegate.Broadcast();
	}
	
	// TrafficLights안에 있는 신호등들의 색깔을 모두 검사
	for (APPTrafficLightBase* TrafficLight : TrafficLights)
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

	if (!TrafficLightEventActors.IsEmpty() && !TrafficLightControllers.IsEmpty())
	{
		for (AActor* TrafficLightEventActor : TrafficLightEventActors)
		{
			IPPTrafficLightEventInterface* TrafficLightEventInterface = CastChecked<IPPTrafficLightEventInterface>(TrafficLightEventActor);
			if (TrafficLightEventInterface != nullptr)
			{
				TrafficLightEventInterface->TrafficLightEvent();
				AudioComponent->Play();
			}
		}

		for (APPTrafficLightController* TrafficLightController : TrafficLightControllers)
		{
			TrafficLightController->SetCanChangeColor(false);
			TrafficLightController->SetTrafficLightControllerState(ETrafficLightControllerState::TLC_WORKING);
		}

		// Disable the Traffic Light Widget
		if (DisableTrafficLightWidgetDelegate.IsBound())
		{
			DisableTrafficLightWidgetDelegate.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Traffic Light Event Actor Found!"));
	}
}

TArray<TObjectPtr<class APPTrafficLightBase>> APPTrafficLightManager::GetTrafficLights() const
{
	return TrafficLights;
}

