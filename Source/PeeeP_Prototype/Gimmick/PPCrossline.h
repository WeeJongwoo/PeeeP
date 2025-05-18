// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPCrossline.generated.h"

DECLARE_DELEGATE_OneParam(FOnCrosslineEventDelegate, bool);

UCLASS()
class PEEEP_PROTOTYPE_API APPCrossline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPCrossline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> CrosslinePillarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> CrosslineHoloMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UBoxComponent> CrosslineCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> CrosslineSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> CrosslineOffSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCrosslineMeshVisibility(bool bVisible);

	FOnCrosslineEventDelegate OnCrosslineEventDelegate;

};
