// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLightStatic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


APPTrafficLightStatic::APPTrafficLightStatic()
{
	TrafficLightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightStaticMesh"));
	SetRootComponent(TrafficLightStaticMesh);
	TrafficLightStaticMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화

	TrafficLightMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("TrafficLightMaterial"));
	TrafficLightStaticMesh->SetMaterial(0, TrafficLightMaterial);

}

void APPTrafficLightStatic::ChangeEmissive(float Type)
{
	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(TrafficLightMaterial, this);

	NewMaterial->SetScalarParameterValue(FName("Emessive_Array_Count"), Type);

	TrafficLightStaticMesh->SetMaterial(0, NewMaterial);
}
