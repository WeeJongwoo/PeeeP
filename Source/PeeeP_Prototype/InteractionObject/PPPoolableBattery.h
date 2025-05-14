// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionObject/PPBattery.h"
#include "Interface/PPPoolableInterface.h"
#include "PPPoolableBattery.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPPoolableBattery : public APPBattery, public IPPPoolableInterface
{
	GENERATED_BODY()
	
public:
	APPPoolableBattery();

	void SetObjectPool(class UPPPoolObject* InObjedtPool) override;
	void ReturnSelf() override;
	void SetActive(bool IsActive) override;

private:

	class UPPPoolObject* ObjectPool;
};
