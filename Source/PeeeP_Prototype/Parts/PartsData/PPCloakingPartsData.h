// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsData/PPPartsDataBase.h"
#include "PPCloakingPartsData.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPCloakingPartsData : public UPPPartsDataBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> CloakingAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInstance> CloakingMaterial;

	UPROPERTY(EditAnywhere)
	float CloakingTime;
};
