// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPHologramBridge.h"

// Sets default values
APPHologramBridge::APPHologramBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh"));
	RootComponent = BridgeMesh;

	BridgeMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void APPHologramBridge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPHologramBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPHologramBridge::OnBridge()
{
	BridgeMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

