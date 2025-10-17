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

	void SyncComboBoxWithCurrentSetting();

protected:
	void InitializeResolutionComboBox();
	void InitializeDisplayModeComboBox();

	UFUNCTION()
	void HideSettingWidget();

	UFUNCTION()
	void OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	UFUNCTION()
	void OnDisplayModeChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	UPROPERTY()
	TObjectPtr<class UGameUserSettings> GameUserSettings;

	UPROPERTY()
	TObjectPtr<class UPPGameInstance> GameInstance;

	// UI Elements

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UComboBoxString> ResolutionComboBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UComboBoxString> DisplayModeComboBox;

	// Audio Settings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> MasterVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> MusicVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> SFXVolumeSlider;

	// Control Settings
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> MouseSensitivitySlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UEditableText> MouseSensitivityText;


	// Widget
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBPSettingExitButton;

	UPROPERTY()
	TArray<FIntPoint> Resolutions;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingWindowAppearAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingWindowDisappearAnim;

	// Text

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> WidgetTitleText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> VideoSettingsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ResolutionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayModeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> SoundSettingsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> MasterVolumeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> MusicVolumeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> SFXVolumeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ControlSettingsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> MouseSensitivityLabelText;

	TMap<FName, UTextBlock*> UIStringMap;

private:

	UFUNCTION()
	void ExitButtonClick();

	// Audio Settings
	UFUNCTION()
	void OnMasterVolumeChanged(float Volume);

	UFUNCTION()
	void OnMusicVolumeChanged(float Volume);

	UFUNCTION()
	void OnSFXVolumeChanged(float Volume);

	// Control Settings
	UFUNCTION()
	void OnMouseSensitivityChanged(float Sensitivity);

	UFUNCTION()
	void OnMouseSensitivityTextChanged(const FText& ValueText);

	UFUNCTION()
	void OnMouseSensitivityTextCommitted(const FText& ValueText, ETextCommit::Type CommitMethod);

	void ApplyMouseSensitivity();

	void MappingTextBoxString();
	void InitializeUIStrings();

	FWidgetAnimationDynamicEvent EndDelegate;
	
};
