// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPauseMenyHUD.h"
#include "UI/Menu/PPMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameMode/PPPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "PPLobbyActor.h"
#include "UI/Menu/PPSettingMenu.h"


UPPPauseMenyHUD::UPPPauseMenyHUD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	LobbyActorClass = APPLobbyActor::StaticClass();
}

void UPPPauseMenyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	/*ContinueButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_ResomeButton")));
	SettingButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_PauseSettingButton")));
	ExitButton = Cast<UPPMenuButtonWidget>(GetWidgetFromName(TEXT("WBP_TitleButton")));*/

	LobbyActor = Cast<APPLobbyActor>(UGameplayStatics::GetActorOfClass(GetWorld(), LobbyActorClass));
	//check(LobbyActor);

	if (WBP_ResomeButton && WBP_ResomeButton->Button)
	{
		UE_LOG(LogTemp, Log, TEXT("Start"));
		WBP_ResomeButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonClick);
		WBP_ResomeButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonHovered);
		WBP_ResomeButton->Button->OnUnhovered.AddDynamic(this, &UPPPauseMenyHUD::ContinueButtonUnhovered);
	}

	if (WBP_PauseSettingButton && WBP_PauseSettingButton->Button)
	{
		WBP_PauseSettingButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonClick);
		WBP_PauseSettingButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonHovered);
		WBP_PauseSettingButton->Button->OnUnhovered.AddDynamic(this, &UPPPauseMenyHUD::SettingButtonUnhovered);
	}

	if (WBP_TitleButton && WBP_TitleButton->Button)
	{
		WBP_TitleButton->Button->OnClicked.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonClick);
		WBP_TitleButton->Button->OnHovered.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonHovered);
		WBP_TitleButton->Button->OnUnhovered.AddDynamic(this, &UPPPauseMenyHUD::ExitButtonUnhovered);
	}

	if (WBPSettingMenu)
	{
		WBPSettingMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UPPPauseMenyHUD::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey.IsValid())
	{
		return FReply::Handled();
	}

	if (PressedKey == EKeys::Escape)
	{
		ContinueButtonClick();
	}

	return FReply::Handled();
}

void UPPPauseMenyHUD::ContinueButtonClick()
{
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->CloseMenu();
	}
}

void UPPPauseMenyHUD::SettingButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("Setting"));
	if (WBPSettingMenu)
	{
		WBPSettingMenu->PlaySettingWindowAppearAnim();
	}
	
}

void UPPPauseMenyHUD::ExitButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLobby"));
}

void UPPPauseMenyHUD::ContinueButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(2);
	if (ResumeButtonHoverStartAnim)
	{
		PlayAnimation(ResumeButtonHoverStartAnim);
	}
}

void UPPPauseMenyHUD::SettingButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(0);
	if (SettingButtonHoverStartAnim)
	{
		PlayAnimation(SettingButtonHoverStartAnim);
	}
}

void UPPPauseMenyHUD::ExitButtonHovered()
{
	UE_LOG(LogTemp, Log, TEXT("Hovered"));
	LobbyActor->ChangeEmessive(3);
	if (TitleButtonHoverStartAnim)
	{
		PlayAnimation(TitleButtonHoverStartAnim);
	}
}

void UPPPauseMenyHUD::ContinueButtonUnhovered()
{
	if (ResumeButtonHoverEndAnim)
	{
		PlayAnimation(ResumeButtonHoverEndAnim);
	}
}

void UPPPauseMenyHUD::SettingButtonUnhovered()
{
	if (SettingButtonHoverEndAnim)
	{
		PlayAnimation(SettingButtonHoverEndAnim);
	}
}

void UPPPauseMenyHUD::ExitButtonUnhovered()
{
	if (TitleButtonHoverEndAnim)
	{
		PlayAnimation(TitleButtonHoverEndAnim);
	}
}
