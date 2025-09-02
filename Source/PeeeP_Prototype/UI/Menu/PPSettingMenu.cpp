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
#include "Components/EditableText.h"
#include "Character/PPCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UIStringRow.h"

void UPPSettingMenu::NativeConstruct()
{	
	Super::NativeConstruct();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();
	GameInstance = Cast<UPPGameInstance>(GetGameInstance());

	if (!GameUserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("GameUserSettings is null!"));
		return;
	}


	// Video Settings Init
	InitializeResolutionComboBox();
	InitializeDisplayModeComboBox();

	// Audio Settings Init
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

	// Control Settings Init
	if (MouseSensitivitySlider)
	{
		MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UPPSettingMenu::OnMouseSensitivityChanged);
	}
	if (MouseSensitivityText)
	{
		MouseSensitivityText->OnTextChanged.AddDynamic(this, &UPPSettingMenu::OnMouseSensitivityTextChanged);
		MouseSensitivityText->OnTextCommitted.AddDynamic(this, &UPPSettingMenu::OnMouseSensitivityTextCommitted);
	}

	if (WBPSettingExitButton && WBPSettingExitButton->Button)
	{
		WBPSettingExitButton->Button->OnClicked.AddDynamic(this, &UPPSettingMenu::ExitButtonClick);
	}

	if (GameInstance)
	{
		// Audio
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

		// Control
		if (MouseSensitivitySlider)
		{
			MouseSensitivitySlider->SetValue(GameInstance->MouseSensitivity);
		}
		if (MouseSensitivityText)
		{
			MouseSensitivityText->SetText(FText::AsNumber(GameInstance->MouseSensitivity));
		}
		GameInstance->ApplySavedAudioSettings();
	}

	EndDelegate.BindDynamic(this, &UPPSettingMenu::HideSettingWidget);
	BindToAnimationFinished(SettingWindowDisappearAnim, EndDelegate);

	// UIString Data Table Test
	if (FUIStringRow* UIStringRow = GameInstance->GetUIStringTable()->FindRow<FUIStringRow>(TEXT("Setting_Title_Video"), TEXT("")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Setting_Title_Video: %s"), *UIStringRow->Text.ToString());
	}

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

void UPPSettingMenu::SyncComboBoxWithCurrentSetting()
{
	EWindowMode::Type CurrentDisplayMode = GameUserSettings->GetFullscreenMode();

	FString ModeText;
	switch (CurrentDisplayMode)
	{
	case EWindowMode::Fullscreen:
		ModeText = TEXT("Fullscreen");
		break;
	case EWindowMode::WindowedFullscreen:
		ModeText = TEXT("Borderless Windowed");
		break;
	case EWindowMode::Windowed:
		ModeText = TEXT("Windowed");
		break;
	default:
		ModeText = TEXT("Unknown");
		break;
	}

	DisplayModeComboBox->SetSelectedOption(ModeText);
}

void UPPSettingMenu::InitializeResolutionComboBox()
{
	Resolutions.Reset();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	// Set Resolution options
	ResolutionComboBox->ClearOptions();
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

void UPPSettingMenu::InitializeDisplayModeComboBox()
{
	if (DisplayModeComboBox)
	{
		DisplayModeComboBox->OnSelectionChanged.AddDynamic(this, &UPPSettingMenu::OnDisplayModeChanged);
		SyncComboBoxWithCurrentSetting();
	}
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

void UPPSettingMenu::OnDisplayModeChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	if (InSelectedItem == TEXT("Fullscreen"))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else if (InSelectedItem == TEXT("Borderless Windowed"))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else if (InSelectedItem == TEXT("Windowed"))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown display mode selected: %s"), *InSelectedItem);
		return;
	}

	GameUserSettings->ApplySettings(false);
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

void UPPSettingMenu::OnMouseSensitivityChanged(float Sensitivity)
{
	MouseSensitivityText->SetText(FText::AsNumber(Sensitivity));
	if (GameInstance)
	{
		GameInstance->MouseSensitivity = Sensitivity;
		ApplyMouseSensitivity();
	}
}

void UPPSettingMenu::OnMouseSensitivityTextChanged(const FText& ValueText)
{
	FString Input = ValueText.ToString();
	FString FilteredInput;

	for (TCHAR Char : Input)
	{
		if (FChar::IsDigit(Char) || Char == '.')
		{
			FilteredInput.AppendChar(Char);
		}
	}

	if (FilteredInput != Input && MouseSensitivityText)
	{
		MouseSensitivityText->SetText(FText::FromString(FilteredInput));
	}
}

void UPPSettingMenu::OnMouseSensitivityTextCommitted(const FText& ValueText, ETextCommit::Type CommitMethod)
{
	FString Input = ValueText.ToString();
	float NewSensitivity = FCString::Atof(*Input);
	// Value check(0.1 <= x <= 100.0)
	NewSensitivity = FMath::Clamp(NewSensitivity, 0.1f, 100.0f);

	if (MouseSensitivitySlider)
	{
		MouseSensitivitySlider->SetValue(NewSensitivity);
	}
	if (MouseSensitivityText)
	{
		MouseSensitivityText->SetText(FText::AsNumber(NewSensitivity));
	}

	if (GameInstance)
	{
		GameInstance->MouseSensitivity = NewSensitivity;
		ApplyMouseSensitivity();
	}
}


void UPPSettingMenu::ApplyMouseSensitivity()
{
	APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	float TempMouseSensitivity = GameInstance->MouseSensitivity;
	if (Player)
	{
		Player->SetMouseSensitivity(TempMouseSensitivity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is not valid!"));
	}
}
