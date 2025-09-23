// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PPGuideWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGuideWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	void RegisterGuideObject(class UPPGuideObjectComponent* GuideObject);
	void UnregisterGuideObject(class UPPGuideObjectComponent* GuideObject);

	void SetGuideActive(bool Active);

private:

	TSet<TWeakObjectPtr<class UPPGuideObjectComponent>> GuideObjectSet;
	
};
