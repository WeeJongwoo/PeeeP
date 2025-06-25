﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "InteractionObject/ETrafficLight.h"
#include "PPTrafficLightController.generated.h"

DECLARE_DELEGATE_OneParam(FOnTrafficLightColorChangedDelegate, ETrafficLightColor);

UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLightController : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrafficLightController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> TrafficLightControllerStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UMaterialInstance> TrafficLightControllerMaterial;

	// Audio Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> IdleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> PowerOnSound;	// 전원이 켜졌을 때 재생되는 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> ColorChangeSound;	// 신호등 변경 시 재생되는 사운드

	// 배터리가 닿는 영역
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UBoxComponent> BatteryTrigger;	

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> SnapPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> ConstraintPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPhysicsConstraintComponent> PhysicsConstraint;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 제어기가 관리하는 신호등 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrafficLightController")
	TArray<TObjectPtr<class APPTrafficLightBase>> TrafficLights;

	// 신호가 바뀌었을 때 신호등들의 색깔에 따른 판정을 하기 위한 TrafficLightManager
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TrafficLightController")
	TObjectPtr<class APPTrafficLightManager> TrafficLightManager;
	
	UPROPERTY()
	uint8 bIsPowerOn : 1;	// 전원이 켜져 있는지 여부를 나타내는 플래그

	UPROPERTY()
	uint8 bCanChangeColor : 1;	// 신호등 색상을 변경할 수 있는지 여부를 나타내는 플래그(퍼즐 클리어 시 전원은 켜진 채 색상 변경 가능 유무를 정하기 위해)

	void Charge() override;

	void ConfigureConstraintSettings();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeTrafficLightColor();

	// TrafficLightsByController를 가져오는 getter함수
	TArray<TObjectPtr<class APPTrafficLightBase>> GetTrafficLightsByController() const;

	FOnTrafficLightColorChangedDelegate OnTrafficLightColorChangedDelegate;
};
