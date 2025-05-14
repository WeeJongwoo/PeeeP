// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPButton.generated.h"

DECLARE_DELEGATE(FOnButtonPressed);

UCLASS()
class PEEEP_PROTOTYPE_API APPButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPButton();

	FOnButtonPressed OnButtonPressedDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> ButtonMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> ButtonCollider;

	UFUNCTION()
	void OnButtonPressed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
