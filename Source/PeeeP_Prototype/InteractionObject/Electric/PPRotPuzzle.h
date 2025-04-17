// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PPRotPuzzle.generated.h"

UENUM()
enum class EPPRotateDirection : uint8
{
	CW,
	CCW
};

UCLASS()
class PEEEP_PROTOTYPE_API APPRotPuzzle : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APPRotPuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Charge() override;

private:

	UPROPERTY(EditAnywhere, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Frame;

	UPROPERTY(EditAnywhere, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Wing;

	UPROPERTY(EditAnywhere, Category = Rotate, meta=(AllowPrivateAccess = "true"))
	float RotationTime;

	UPROPERTY(EditAnywhere, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	EPPRotateDirection RotationDirection;

	UPROPERTY(EditDefaultsOnly, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> DeactivedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInterface> ActivedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = Rotate, meta = (AllowPrivateAccess = "true"))
	uint8 IsBroken : 1;

	bool bIsRotating;
	float CurrentRotatingTime;

	FRotator StartRotation;
	FRotator TargetRotation;
};
