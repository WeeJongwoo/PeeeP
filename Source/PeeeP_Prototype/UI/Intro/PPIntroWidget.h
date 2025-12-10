// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPIntroWidget.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPIntroWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void StartGame();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaPlayer> VideoMedia;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaSource> VideoMediaSource;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaSource> VideoMediaLoopSource;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMediaTexture> VideoMediaTexture;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UWorld> LevelToOpen;


public:

	void PlayVideo();

	UFUNCTION()
	void PlayLoop();

};
