// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPSettingMenu.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPSettingMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void PlaySettingWindowAppearAnim();
	void PlaySettingWindowDisappearAnim();

protected:
	void InitializeResolutionComboBox();

	UFUNCTION()
	void HideSettingWidget();

	UFUNCTION()
	void OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	UPROPERTY()
	TObjectPtr<class UGameUserSettings> GameUserSettings;

	UPROPERTY()
	TObjectPtr<class UPPGameInstance> GameInstance;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UComboBoxString> ResolutionComboBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> MasterVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> MusicVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> SFXVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBPSettingExitButton;

	UPROPERTY()
	TArray<FIntPoint> Resolutions;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingWindowAppearAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingWindowDisappearAnim;

private:
	UFUNCTION()
	void ExitButtonClick();

	UFUNCTION()
	void OnMasterVolumeChanged(float Volume);

	UFUNCTION()
	void OnMusicVolumeChanged(float Volume);

	UFUNCTION()
	void OnSFXVolumeChanged(float Volume);

	FWidgetAnimationDynamicEvent EndDelegate;
	
};
