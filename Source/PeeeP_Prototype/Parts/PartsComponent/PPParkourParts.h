// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsComponent/PPPartsBase.h"
#include "PPParkourParts.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPParkourParts : public UPPPartsBase
{
	GENERATED_BODY()

public:
	UPPParkourParts();
	
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void CleanUpParts() override;

	void ChargStart();
	void TickJumpCharge();
	void Jump();

	int8 bIsIsCharging : 1;

	float DefaultJumpZVelocity;
	float DefaultMaxWalkSpeed;

	float ParkourSpeedMultiplier;
	float ParkourJumpMultiplier;

	int32 CurrentJumpLevel;
	int32 MaxJumpLevel;

	float TimePerChargeLevel;

	float PreviousJumpChargingTime;
	float CurrentJumpChargingTime;

	float TotalJumpChargingTime;
	float MaxJumpChargingTime;

	TObjectPtr<class UNiagaraSystem> JumpChargingEffect;

	TObjectPtr<class UNiagaraComponent> ParkourEffectComponent;
};
