// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPTrainAnimInstance.h"

UPPTrainAnimInstance::UPPTrainAnimInstance()
{
}

void UPPTrainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	/*Owner = Cast<APPTrain>(GetOwningActor());
	if (Owner)
	{
		
	}*/
}

void UPPTrainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
}
