// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPGameMenuHUD.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PPLobbyActor.h"
#include "GameMode/PPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Menu/PPSettingMenu.h"

UPPGameMenuHUD::UPPGameMenuHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LobbyActorClass = APPLobbyActor::StaticClass();
}

void UPPGameMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();

	/*StartButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_StartButton")));
	LoadButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_LoadButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_SettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ExitButton")));*/

	LobbyActor = Cast<APPLobbyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), LobbyActorClass));

	if (WBP_StartButton && WBP_StartButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		WBP_StartButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::StartButtonClick);
		WBP_StartButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::StartButtonHover);
		WBP_StartButton->Button->OnUnhovered.AddDynamic(this, &UPPGameMenuHUD::StartButtonHoverEnd);
	}

	if (WBP_LoadButton && WBP_LoadButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Load"));
		WBP_LoadButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::LoadButtonClick);
		WBP_LoadButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::LoadButtonHover);
		WBP_LoadButton->Button->OnUnhovered.AddDynamic(this, &UPPGameMenuHUD::LoadButtonHoverEnd);
	}

	if (WBP_SettingButton && WBP_SettingButton->Button)
	{
		WBP_SettingButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::SettingButtonClick);
		WBP_SettingButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::SettingButtonHover);
		WBP_SettingButton->Button->OnUnhovered.AddDynamic(this, &UPPGameMenuHUD::SettingButtonHoverEnd);
	}

	if (WBP_ExitButton && WBP_ExitButton->Button)
	{
		WBP_ExitButton->Button->OnClicked.AddDynamic(this, &UPPGameMenuHUD::ExitButtonClick);
		WBP_ExitButton->Button->OnHovered.AddDynamic(this, &UPPGameMenuHUD::ExitButtonHover);
		WBP_ExitButton->Button->OnUnhovered.AddDynamic(this, &UPPGameMenuHUD::ExitButtonHoverEnd);
	}

	if (WBP_SettingMenu)
	{
		WBP_SettingMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	if (LogoLoopAnim)
	{
		PlayAnimation(LogoLoopAnim, 0.0f, 9999);
	}
}

void UPPGameMenuHUD::StartButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("StartGame"));

	UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->ClearInventoryPartsArray();
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage1"));
}

void UPPGameMenuHUD::LoadButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("LoadOpen"));
}

void UPPGameMenuHUD::SettingButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("SettingOpen"));
	WBP_SettingMenu->SetVisibility(ESlateVisibility::Visible);
	WBP_SettingMenu->PlaySettingWindowAppearAnim();
}

void UPPGameMenuHUD::ExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UPPGameMenuHUD::StartButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	PlayAnimation(StartButtonHoverStartAnim);
	LobbyActor->ChangeEmessive(2);
}

void UPPGameMenuHUD::LoadButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	PlayAnimation(LoadButtonHoverStartAnim);
	LobbyActor->ChangeEmessive(1);
}

void UPPGameMenuHUD::SettingButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	PlayAnimation(SettingButtonHoverStartAnim);
	LobbyActor->ChangeEmessive(0);
}

void UPPGameMenuHUD::ExitButtonHover()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	PlayAnimation(ExitButtonHoverStartAnim);
	LobbyActor->ChangeEmessive(3);
}

void UPPGameMenuHUD::StartButtonHoverEnd()
{
	PlayAnimation(StartButtonHoverEndAnim);
}

void UPPGameMenuHUD::LoadButtonHoverEnd()
{
	PlayAnimation(LoadButtonHoverEndAnim);
}

void UPPGameMenuHUD::SettingButtonHoverEnd()
{
	PlayAnimation(SettingButtonHoverEndAnim);
}

void UPPGameMenuHUD::ExitButtonHoverEnd()
{
	PlayAnimation(ExitButtonHoverEndAnim);
}
