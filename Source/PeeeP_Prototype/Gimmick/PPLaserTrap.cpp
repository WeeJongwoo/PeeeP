// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPLaserTrap.h"

// Sets default values
APPLaserTrap::APPLaserTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPLaserTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPLaserTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPLaserTrap::OnLaserPressed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

