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

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void CleanUpParts() override;

	void StartCloaking();
	void StopCloaking();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInstance> DefaultMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInstance> CloakingMaterial;

};
