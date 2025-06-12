﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPHologramBridgeManager.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPHologramBridgeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPHologramBridgeManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Button")
	TObjectPtr<class APPButton> Button1;
	
	UPROPERTY(EditAnywhere, Category = "Button")
	TObjectPtr<class APPButton> Button2;

	uint8 bButton1State : 1;
	uint8 bButton2State : 1;
	uint8 bIsClear : 1;
	bool bSequencePlayed;

	void SetClear();

	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere, Category = "Bridge")
	TObjectPtr<class APPHologramBridge> HologramBridge;

	UPROPERTY(EditAnywhere, Category = "Bridge")
	TObjectPtr<class ULevelSequence> ClearSequence;

	TObjectPtr<class ULevelSequencePlayer> ClearSequencePlayer;

	void SwitchToLightCam();

	void RevertCam();
	
	TObjectPtr<class AActor> DefaultCamActor;

	UPROPERTY(EditAnywhere, Category = "LightCam")
	TObjectPtr<class AActor> LightCamActor;

	UPROPERTY(EditAnywhere, Category = "LightCam")
	float CameraViewDuration;

	UPROPERTY(EditAnywhere, Category = "LightCam")
	float CameraBlendDuration;

	UPROPERTY(EditAnywhere, Category = "LightCam")
	TArray<TObjectPtr<class AActor>> BridgeButtonSignActors;

	UPROPERTY(EditAnywhere, Category = "LightCam")
	TObjectPtr<class UMaterialInstance> BridgeButtonSignMaterial;

	FTimerHandle LightCamTimerHandle;
	FTimerHandle LightOnTimerHandle;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	void ChangeBridgeButtonSignMaterial(float Type);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void SequenceFinished();

	
};

