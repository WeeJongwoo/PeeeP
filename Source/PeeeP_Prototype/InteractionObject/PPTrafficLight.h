// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionObject/ETrafficLight.h"
#include "PPTrafficLight.generated.h"

DECLARE_DELEGATE(FCheckTrafficLigthColorDelegate)

UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrafficLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> TrafficLightStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class UMaterialInstance> TrafficLightMaterial;

	// 여기 헤더에는 enum 헤더 없이 하고싶은데 어떤 타입을 써야될까
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrafficLight")
	ETrafficLightType TrafficLightType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrafficLight")
	ETrafficLightColor CurrentTrafficLightColor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ETrafficLightColor GetCurrentTrafficLightColor() const;
	ETrafficLightType GetTrafficLightType() const;

	void ChangeColor(ETrafficLightColor NewTrafficLightColor);

	void ChangeEmissive(float Type);

	FCheckTrafficLigthColorDelegate CheckTrafficLigthColorDelegate;
};
