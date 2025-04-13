// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPTrafficLightController.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionObject/PPTrafficLight.h"
#include "InteractionObject/ETrafficLight.h"
#include "InteractionObject/PPTrafficLightManager.h"

// Sets default values
APPTrafficLightController::APPTrafficLightController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrafficLightControllerStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightControllerStaticMesh"));
	SetRootComponent(TrafficLightControllerStaticMesh);
	TrafficLightControllerStaticMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화
	TrafficLightControllerStaticMesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// 플레이어의 전기 방출을 받기 위한 콜리전 프로필 세팅.

	TrafficLightControllerMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("TrafficLightControllerMaterial"));
	TrafficLightControllerStaticMesh->SetMaterial(0, TrafficLightControllerMaterial);

	bIsPowerOn = false;	// 초기값은 꺼져있음
}

// Called when the game starts or when spawned
void APPTrafficLightController::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPTrafficLightController::Charge()
{
	UE_LOG(LogTemp, Log, TEXT("TEST"));

	// 디버그용: 초기 bIsPowerOn이 false일 경우 전원만 킴(신호 변화 X)
	if (!bIsPowerOn)
	{
		bIsPowerOn = true;
		UE_LOG(LogTemp, Log, TEXT("Powered On"));
		return;
	}

	// 이후 신호 변화
	ChangeTrafficLightColor();
}

// Called every frame
void APPTrafficLightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPTrafficLightController::ChangeTrafficLightColor()
{
	// 신호등 색상 변경
	// 타입에 따라 변경 방식이 다름
	for (TObjectPtr<AActor> TrafficLightActor : TrafficLights)
	{
		APPTrafficLight* TrafficLight = Cast<APPTrafficLight>(TrafficLightActor);
		if (IsValid(TrafficLight))
		{
			UE_LOG(LogTemp, Log, TEXT("Cast Completed"));
			ETrafficLightType TrafficLightType = TrafficLight->GetTrafficLightType();
			ETrafficLightColor CurrentTrafficLightColor = TrafficLight->GetCurrentTrafficLightColor();
			switch (TrafficLightType)
			{
				//두 가지 색상의 신호등
			case ETrafficLightType::TL_TWO:
				switch (CurrentTrafficLightColor)
				{
				case ETrafficLightColor::TC_GREEN:
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
					break;
				case ETrafficLightColor::TC_RED:
					CurrentTrafficLightColor = ETrafficLightColor::TC_GREEN;
					break;
				default:
					break;
				}
				break;
				// 세가지 색상의 신호등
			case ETrafficLightType::TL_THREE:
				if (CurrentTrafficLightColor == ETrafficLightColor::TC_GREEN)
				{
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
				}
				else if (CurrentTrafficLightColor == ETrafficLightColor::TC_OFF)
				{
					// 신호등이 꺼져있을 때(현재 이 기능은 사용하지 않음)
				}
				else
				{
					uint8 TempColorNum = static_cast<uint8>(CurrentTrafficLightColor);
					TempColorNum--;
					CurrentTrafficLightColor = static_cast<ETrafficLightColor>(TempColorNum);
				}
				break;

			default:
				break;
			}
			// 최종적으로 해당하는 색깔로 변경
			TrafficLight->ChangeColor(CurrentTrafficLightColor);
		}
	}
}

TArray<TObjectPtr<class AActor>> APPTrafficLightController::GetTrafficLightsByController() const
{
	return TrafficLights;
}

