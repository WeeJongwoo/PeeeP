// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPBattery.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APPBattery::APPBattery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BatteryStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryStaticMesh"));
	SetRootComponent(BatteryStaticMesh);
	BatteryStaticMesh->SetSimulatePhysics(true);

	// Initialize the Gameplay Tag Container
	MyTags.AddTag(FGameplayTag::RequestGameplayTag(FName("TrafficLight.Battery")));
	BatteryTag = FGameplayTag::RequestGameplayTag(FName("TrafficLight.Battery"));
}

// Called when the game starts or when spawned
void APPBattery::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPBattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* APPBattery::GetBatteryStaticMesh() const
{
	return BatteryStaticMesh;
}

