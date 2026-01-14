// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Intro/PPIntroWidget.h"
#include "PPCreditWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPCreditWidget : public UPPIntroWidget
{
	GENERATED_BODY()
	
public:
	UPPCreditWidget(const FObjectInitializer& ObjectInitializer);
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> CreditAnim;

private:
	TObjectPtr<class USoundBase> CreditMusic;
};
