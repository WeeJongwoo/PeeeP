// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLightSkeletal.h"
#include "Components/SkeletalMeshComponent.h"

APPTrafficLightSkeletal::APPTrafficLightSkeletal()
{
	TrafficLightSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TrafficLightSkeletalMesh"));
	SetRootComponent(TrafficLightSkeletalMesh);
	TrafficLightSkeletalMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화

	TrafficLightMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("TrafficLightMaterial"));
	TrafficLightSkeletalMesh->SetMaterial(0, TrafficLightMaterial);

	TrafficLightType = ETrafficLightType::TL_THREE_WITH_BAR;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ElectricObject/TrafficLight/ABP_TrafficLightSkeletal.ABP_TrafficLightSkeletal_C"));
	if (AnimInstanceClassRef.Class)
	{
		TrafficLightSkeletalMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

void APPTrafficLightSkeletal::ChangeColor(ETrafficLightColor NewTrafficLightColor)
{
	CurrentTrafficLightColor = NewTrafficLightColor;

	ChangeEmissive(static_cast<float>(CurrentTrafficLightColor));
	UE_LOG(LogTemp, Warning, TEXT("CurrentTrafficLightColor: %s"), *UEnum::GetValueAsString(CurrentTrafficLightColor));
}

void APPTrafficLightSkeletal::ChangeEmissive(float Type)
{
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(TrafficLightMaterial, this);
	NewMaterial->SetScalarParameterValue(FName("Emessive_Array_Count"), Type);
	TrafficLightSkeletalMesh->SetMaterial(0, NewMaterial);
}

void APPTrafficLightSkeletal::PlayAnimation()
{

}
