// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPTrafficLightController.h"

// Sets default values
APPTrafficLightController::APPTrafficLightController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPTrafficLightController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPTrafficLightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

