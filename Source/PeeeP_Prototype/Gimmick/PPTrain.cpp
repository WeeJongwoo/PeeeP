// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPTrain.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
APPTrain::APPTrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
	TrainMesh->SetupAttachment(Root);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void APPTrain::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->SetSound(MoveSound);
	
}

// Called every frame
void APPTrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

