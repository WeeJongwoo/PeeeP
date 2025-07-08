// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPSettingMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ComboBoxString.h"
#include "Animation/WidgetAnimation.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"


void UPPSettingMenu::NativeConstruct()
{	
	Super::NativeConstruct();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	InitializeResolutionComboBox();

	if (WBPSettingExitButton && WBPSettingExitButton->Button)
	{
		WBPSettingExitButton->Button->OnClicked.AddDynamic(this, &UPPSettingMenu::ExitButtonClick);
	}

}

void UPPSettingMenu::PlaySettingWindowAppearAnim()
{
	if (SettingWindowAppearAnim)
	{
		PlayAnimation(SettingWindowAppearAnim);
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
	SetVisibility(ESlateVisibility::Hidden);
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
	GetWorld()->GetTimerManager().ClearTimer(SettingWindowDisappearAnimTimerHandle); // Clear any previous timer
	GetWorld()->GetTimerManager().SetTimer(SettingWindowDisappearAnimTimerHandle, this, &UPPSettingMenu::HideSettingWidget, SettingWindowDisappearAnim->GetEndTime(), false);
}
