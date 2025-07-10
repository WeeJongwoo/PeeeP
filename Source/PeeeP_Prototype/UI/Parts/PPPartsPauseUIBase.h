// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPPartsPauseUIBase.generated.h"


DECLARE_DELEGATE(FPartsUIEndDelegate);

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPPartsPauseUIBase : public UUserWidget
{
	GENERATED_BODY()
	
	
	UPPPartsPauseUIBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION()
	void End();

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UImage> Parts_Info;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UTexture2D>> PartsInfoImages;

	int16 CurrentPartsIndex = 0;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetPartsInfoImage(const int16 InCurrentPartsIndex);

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> PartsUIWidgetAppearAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> PartsUIWidgetDisappearAnim;

	FWidgetAnimationDynamicEvent DisappearAnimEndDelegate;

public:

	FPartsUIEndDelegate EndUIDelegate;

	void SetPartsInfoImageArray(const TArray<TSoftObjectPtr<UTexture2D>>& InPartsInfo);



};
