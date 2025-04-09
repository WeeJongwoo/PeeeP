// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPTrafficLightManager.h"

// Sets default values
APPTrafficLightManager::APPTrafficLightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPTrafficLightManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPTrafficLightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

