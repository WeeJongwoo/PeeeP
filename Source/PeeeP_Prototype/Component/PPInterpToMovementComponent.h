// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InterpToMovementComponent.h"
#include "PPInterpToMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPInterpToMovementComponent : public UInterpToMovementComponent
{
	GENERATED_BODY()


public:
	void ResumeMovement();
	
};
