// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPLoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPLoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPPLoadingWidget(const FObjectInitializer& ObjectInitializer);


	void PlayFadeInAnimation();
	void PlayFadeOutAnimation();

	const TObjectPtr<class UWidgetAnimation> GetFadeInAnimation() const { return FadeIn; }
	const TObjectPtr<class UWidgetAnimation> GetFadeOutAnimation() const { return FadeOut; }

	void SetOnFadeInFinished(FSimpleDelegate InDelegate);
	void SetOnFadeOutFinished(FSimpleDelegate InDelegate);


private:
	UFUNCTION()
	void OnFadeInFinishedCallback();

	UFUNCTION()
	void OnFadeOutFinishedCallback();

private:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeOut;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FadeImage;
	
	FSimpleDelegate OnFadeInFinished;
	FSimpleDelegate OnFadeOutFinished;
	
};
