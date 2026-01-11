// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PPGuideWorldSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FGuideActiveDelegate);
DECLARE_MULTICAST_DELEGATE(FGuideHiddenDelegate);


UCLASS()
class PEEEP_PROTOTYPE_API UPPGuideWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	void RegisterGuideObject(class UPPGuideObjectComponent* GuideObject);
	void UnregisterGuideObject(class UPPGuideObjectComponent* GuideObject);

	void GuideActive();

	FGuideActiveDelegate OnGuideActiveDelegate;
	FGuideHiddenDelegate OnGuideHiddenDelegate;

private:

	TSet<TWeakObjectPtr<class UPPGuideObjectComponent>> GuideObjectSet;
	
	FTimerHandle GuideTimerHandle;

};
