// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APPPlayerController();
	
	void OpenMenu();
	void CloseMenu();

	void OpenPartsPauseWidget(TSubclassOf<class UUserWidget> InWidgetClass, const TArray<TSoftObjectPtr<UTexture2D>>& InPartsInfo);
	void ClosePartsPauseWidget();

protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UUserWidget> PauseUIClass;

	TObjectPtr<class UUserWidget> PauseUI;
	// ���� Ŭ������ ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UPPInGameUIMain> InGameUIMainClass;
	// ���� ������ ������ ����Ű�� ������
	UPROPERTY()
	TObjectPtr<class UPPInGameUIMain> InGameUIMain;

	TObjectPtr<class UUserWidget> CurrentPauseWidget;

	void SetUIInputMode(TObjectPtr<class UUserWidget> FocusTarget);
	void SetGameInputMode();

private:
	TObjectPtr<class UUserWidget> ActiveTipWidget;

	void ShowLevelStartTipWidget(const class UPPLevelData* LevelDataAsset);
	
	UFUNCTION()
	void DisplayLevelStartTipWidget();
	UFUNCTION()
	void RemoveLevelStartTipWidget();

	FTimerHandle TipWidgetAppearTimerHandle;
	FTimerHandle TipWidgetDisappearTimerHandle;


};
