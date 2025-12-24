// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPResetSaveWindow.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnResetSaveConfirmed);

UCLASS()
class PEEEP_PROTOTYPE_API UPPResetSaveWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnResetSaveConfirmed OnResetSaveConfirmedDelegate;
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UButton> ButtonYes;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UButton> ButtonNo;

protected:

	UFUNCTION()
	void ButtonYesClick();

	UFUNCTION()
	void ButtonNoClick();
};
