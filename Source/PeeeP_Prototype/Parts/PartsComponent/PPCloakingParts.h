// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsComponent/PPPartsBase.h"
#include "PPCloakingParts.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPCloakingParts : public UPPPartsBase
{
	GENERATED_BODY()
	
public:

	UPPCloakingParts();

	virtual void PartsInit(TObjectPtr<class UPPPartsDataBase> InPartsData) override;

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void CleanUpParts() override;

	void StartCloaking();
	void StopCloaking();
	void SetCloakingState();
	void RestoreCloakingState();

	TObjectPtr<class UMaterialInstance> CloakingMaterial;

	int32 NumMaterials;

	bool bCanCloaking;

	float CloakingTime;
	float Cooldown;

	FTimerHandle CloakingTimerHandle;
	FTimerHandle CloakingCooldownTimerHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> CloakingSoundComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> StartSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> EndSound;
};
