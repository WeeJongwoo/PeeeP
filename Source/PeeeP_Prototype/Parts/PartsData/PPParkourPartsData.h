// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsData/PPPartsDataBase.h"
#include "PPParkourPartsData.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPParkourPartsData : public UPPPartsDataBase
{
	GENERATED_BODY()
	
public:
	UPPParkourPartsData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ParkourSpeedMultiplierValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> ChargingJumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class UNiagaraSystem>> JumpChargingEffect;
};
