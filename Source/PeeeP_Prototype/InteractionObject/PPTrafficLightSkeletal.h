// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionObject/PPTrafficLightBase.h"
#include "PPTrafficLightSkeletal.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLightSkeletal : public APPTrafficLightBase
{
	GENERATED_BODY()
	
public:
	APPTrafficLightSkeletal();

protected:
	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> TrafficLightSkeletalMesh;

public:
	virtual void ChangeEmissive(float Type) override;

	void PlayAnimation();
};
