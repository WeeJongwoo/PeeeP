// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionObject/PPTrafficLightBase.h"
#include "PPTrafficLightStatic.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLightStatic : public APPTrafficLightBase
{
	GENERATED_BODY()
	
public:
	APPTrafficLightStatic();

protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> TrafficLightStaticMesh;
	
public:
	virtual void ChangeEmissive(float Type) override;
};
