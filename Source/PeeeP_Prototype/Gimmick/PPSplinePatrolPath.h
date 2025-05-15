// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPSplinePatrolPath.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPSplinePatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPSplinePatrolPath();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	TObjectPtr<class USplineComponent> Spline;

	UPROPERTY(EditAnywhere, Category = "Spline")
	uint8 bIsLoop : 1;

	UPROPERTY(EditAnywhere, Category = "Spline")
	TSubclassOf<class AActor> ActorToMoveClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", Meta = (AllowPrivateAccess = "true"))
	uint8 bCanMoveActor;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveActor();

private:
	void SetActorRotationAndLocation(float Distance);
	float StartTime;
	bool bIsMovingForward;
	class AActor* ActorToMove;

};
