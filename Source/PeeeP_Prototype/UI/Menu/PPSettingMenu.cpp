// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPSettingMenu.h"
#include "GameMode/PPGameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ComboBoxString.h"
#include "Animation/WidgetAnimation.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"


void UPPSettingMenu::NativeConstruct()
{	
	Super::NativeConstruct();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	GameInstance = Cast<UPPGameInstance>(GetGameInstance());

	InitializeResolutionComboBox();

	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UPPSettingMenu::OnMasterVolumeChanged);
	}
	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UPPSettingMenu::OnMusicVolumeChanged);
	}
	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UPPSettingMenu::OnSFXVolumeChanged);
	}

	if (WBPSettingExitButton && WBPSettingExitButton->Button)
	{
		WBPSettingExitButton->Button->OnClicked.AddDynamic(this, &UPPSettingMenu::ExitButtonClick);
	}

	if (GameInstance)
	{
		if (MasterVolumeSlider)
		{
			MasterVolumeSlider->SetValue(GameInstance->MasterVolume);
		}
		if (MusicVolumeSlider)
		{
			MusicVolumeSlider->SetValue(GameInstance->MusicVolume);
		}
		if (SFXVolumeSlider)
		{
			SFXVolumeSlider->SetValue(GameInstance->SFXVolume);
		}
		GameInstance->ApplySavedAudioSettings();
	}

	EndDelegate.BindDynamic(this, &UPPSettingMenu::HideSettingWidget);
	BindToAnimationFinished(SettingWindowDisappearAnim, EndDelegate);
}

void UPPSettingMenu::PlaySettingWindowAppearAnim()
{
	if (SettingWindowAppearAnim)
	{
		this->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(SettingWindowAppearAnim);
		this->bIsFocusable = true;
	}
}

void UPPSettingMenu::PlaySettingWindowDisappearAnim()
{
	if (SettingWindowDisappearAnim)
	{
		PlayAnimation(SettingWindowDisappearAnim);
	}
}

void UPPSettingMenu::InitializeResolutionComboBox()
{
	Resolutions.Reset();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	// Set Resolution options
	ResolutionComboBox->ClearOptions();
	for (const FIntPoint& Resolution : Resolutions)
	{
		// 1920x1080, 1280x720, etc.
		FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}

	// find current resolution
	const auto CurrentResolution = GameUserSettings->GetScreenResolution();
	const auto SelectedIndex = Resolutions.IndexOfByPredicate(
		[&CurrentResolution](const FIntPoint& Res)
		{
		return Res.X == CurrentResolution.X && Res.Y == CurrentResolution.Y;
		});
	check(SelectedIndex >= 0);
	ResolutionComboBox->SetSelectedIndex(SelectedIndex);

	// Listen for changes
	ResolutionComboBox->OnSelectionChanged.Clear();
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UPPSettingMenu::OnResolutionChanged);
}

void UPPSettingMenu::HideSettingWidget()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->bIsFocusable = false;
}

void UPPSettingMenu::OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	const auto SelectedIndex = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	GameUserSettings->SetScreenResolution(SelectedIndex);
	GameUserSettings->ApplyResolutionSettings(false);
}

void UPPSettingMenu::ExitButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("Clicked"));
	PlaySettingWindowDisappearAnim();
}

void UPPSettingMenu::OnMasterVolumeChanged(float Value)
{
	if (GameInstance)
	{
		GameInstance->MasterVolume = Value;
		GameInstance->ApplySavedAudioSettings();
	}
}

void UPPSettingMenu::OnMusicVolumeChanged(float Volume)
{
	if (GameInstance)
	{
		GameInstance->MusicVolume = Volume;
		GameInstance->ApplySavedAudioSettings();
	}
}

void UPPSettingMenu::OnSFXVolumeChanged(float Volume)
{
	if (GameInstance)
	{
		GameInstance->SFXVolume = Volume;
		GameInstance->ApplySavedAudioSettings();
	}
}
