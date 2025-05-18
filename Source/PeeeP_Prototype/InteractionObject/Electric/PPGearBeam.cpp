// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPGearBeam.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APPGearBeam::APPGearBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));
}

// Called when the game starts or when spawned
void APPGearBeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPGearBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPGearBeam::Charge()
{
	if (IsValid(Mesh))
	{
		Mesh->SetMaterial(0, Material);
		UE_LOG(LogTemp, Log, TEXT("Charge"));
	}
}

