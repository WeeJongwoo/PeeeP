// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPBattery.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPBattery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPBattery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> BatteryStaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
