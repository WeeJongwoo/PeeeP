// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPLaserTrap.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPLaserTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPLaserTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<class UStaticMeshComponent> LaserMesh;

	TObjectPtr<class UBoxComponent> LaserCollider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnLaserPressed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
