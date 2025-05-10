// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPHologramBridge.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPHologramBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPHologramBridge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> BridgeMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnBridge();
};
