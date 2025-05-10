// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PP_ElectricMovingPlatform.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APP_ElectricMovingPlatform : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_ElectricMovingPlatform();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InterpToMovement")
	TObjectPtr<class UInterpToMovementComponent> InterpToMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> NewMaterial;

	// Start and End position for the InterpToMovement component
	UPROPERTY(EditAnywhere, Category="InterpToMovement")
	TObjectPtr<class USceneComponent> StartPosition;

	UPROPERTY(EditAnywhere, Category = "InterpToMovement")
	TObjectPtr<class USceneComponent> EndPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InterpToMovement")
	float StopTime;
	
	FTimerHandle StopTimerHandle;

	bool bIsCharged;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void Charge() override;

	void MoveBackToStart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnReverse(const FHitResult& ImpactResult, float Time);
};
