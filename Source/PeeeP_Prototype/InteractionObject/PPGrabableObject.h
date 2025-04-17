// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPGrabableObject.generated.h"

DECLARE_DELEGATE(FGrabReleaseDelegate)

UCLASS()
class PEEEP_PROTOTYPE_API APPGrabableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPGrabableObject();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsGrabbed : 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsGrabbed(uint8 bNewBool);
	uint8 GetIsGrabbed() const;

	FGrabReleaseDelegate GrabReleaseDelegate;
};
