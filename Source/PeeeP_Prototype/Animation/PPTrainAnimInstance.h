// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPTrainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPTrainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPPTrainAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Animation")
	TObjectPtr<class APPTrain> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Train Animation")
	uint8 bIsMoving : 1;


};
