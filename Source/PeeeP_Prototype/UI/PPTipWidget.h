// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UI/PPWidgetAnimationInterface.h"
#include "PPTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPTipWidget : public UUserWidget, public IPPWidgetAnimationInterface
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void SetTipTitleText(const FName& TitleKey);
	void SetTipContentsText(const FName& ContentsKey);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TipTitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TipContentsText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> TipAppearAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> TipDisappearAnim;

public:
	// Interface implementation
	virtual void PlayWidgetAnimationIn() override;
	virtual void PlayWidgetAnimation() override;
	virtual void PlayWidgetAnimationOut() override;

	// Delegate for animation finished
	FWidgetAnimationDynamicEvent OnTipDisappearAnimFinished;
};
