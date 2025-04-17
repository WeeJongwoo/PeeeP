// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLight.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APPTrafficLight::APPTrafficLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrafficLightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightStaticMesh"));
	SetRootComponent(TrafficLightStaticMesh);
	TrafficLightStaticMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화

	TrafficLightMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("TrafficLightMaterial"));
	TrafficLightStaticMesh->SetMaterial(0, TrafficLightMaterial);

	CurrentTrafficLightColor = ETrafficLightColor::TC_GREEN;	// 기본값은 초록색 TC_GREEN
	TrafficLightType = ETrafficLightType::TL_THREE;				// 신호등 종류, 기본값은 3가지 신호등 TL_THREE
}

// Called when the game starts or when spawned
void APPTrafficLight::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 초기 신호등 색상 결정
	ChangeEmissive(static_cast<float>(CurrentTrafficLightColor));


}

// Called every frame
void APPTrafficLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ETrafficLightColor APPTrafficLight::GetCurrentTrafficLightColor() const
{
	return CurrentTrafficLightColor;
}

ETrafficLightType APPTrafficLight::GetTrafficLightType() const
{
	return TrafficLightType;
}

void APPTrafficLight::ChangeColor(ETrafficLightColor NewTrafficLightColor)
{
	CurrentTrafficLightColor = NewTrafficLightColor;
	ChangeEmissive(static_cast<float>(CurrentTrafficLightColor));
	UE_LOG(LogTemp, Warning, TEXT("CurrentTrafficLightColor: %s"), *UEnum::GetValueAsString(CurrentTrafficLightColor));
}

void APPTrafficLight::ChangeEmissive(float Type)
{
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(TrafficLightMaterial, this);

	NewMaterial->SetScalarParameterValue(FName("Emessive_Array_Count"), Type);

	TrafficLightStaticMesh->SetMaterial(0, NewMaterial);
}

