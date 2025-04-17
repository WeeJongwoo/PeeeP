// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionObject/ETrafficLight.h"
#include "PPTrafficLightManager.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrafficLightManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 매니저가 관리하는 신호등 제어기 배열
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrafficLights")
	TArray<TObjectPtr<class APPTrafficLightController>> TrafficLightControllers;

	// 매니저가 관리하는 신호등 배열
	// 매니저가 신호등의 상태를 확인하여 이후 처리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TrafficLights")
	TArray<TObjectPtr<class APPTrafficLightBase>> TrafficLights;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CheckTrafficLightsColor(ETrafficLightColor Color);

	void StartEvent();
};
