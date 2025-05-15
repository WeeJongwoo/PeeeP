// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/PPTrafficLightEventInterface.h"
#include "PPTrainSpline.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPTrainSpline : public AActor, public IPPTrafficLightEventInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrainSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	TObjectPtr<class USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineController")
	uint8 bIsLoop : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineController")
	TSubclassOf<class AActor> ActorToMoveClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SplineController", Meta = (AllowPrivateAccess = "true"))
	uint8 bCanMoveActor : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	float CurrentDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineController")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineController")
	float DecelerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineController")
	float StopDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	float TotalMoveTime;

private:
	TObjectPtr<class AActor> ActorToMove;

	float StartTime;
	bool bIsMovingForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SplineController", Meta = (AllowPrivateAccess = "true"))
	uint8 bPendingStop : 1;
	
	bool bIsStopping;

	float CurrentSpeed;

	void SetMoveActorLocation(float Distance);
	void SetMoveActorRotation(float Distance);
	void SetMoveActorRotationAndLocation(float Distance);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 외부에서 스플라인을 컨트롤 할 수 있는 곳
	void StopActorMoveEaseIn();

public:
	// IPPTrafficLightEventInterface 
	virtual void TrafficLightEvent() override;

};
