// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PPGearBeam.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPGearBeam : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPGearBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInstance> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Charge() override;
};
