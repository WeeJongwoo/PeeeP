﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "UI/Menu/PPPauseMenyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/PPInGameUIMain.h"
#include "PPPlayerState.h"
#include "UI/Parts/PPPartsPauseUIBase.h"


APPPlayerController::APPPlayerController()
{
	SetTickableWhenPaused(true);

	static ConstructorHelpers::FClassFinder<UPPPauseMenyHUD> PauseMenuRef(TEXT("/Game/UI/MenuHUD/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuRef.Class)
	{
		PauseUIClass = PauseMenuRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPPInGameUIMain> InGameUIClassRef(TEXT("/Game/UI/WBP_InGameMain.WBP_InGameMain_C"));
	if (InGameUIClassRef.Class)
	{
		InGameUIMainClass = InGameUIClassRef.Class;
	}
}

void APPPlayerController::OpenMenu()
{
	SetPause(true);
	//GetWorld()->GetWorldSettings()->SetTimeDilation(0.03f);

	PauseUI->SetVisibility(ESlateVisibility::Visible);
	
	SetUIInputMode(PauseUI);
}

void APPPlayerController::CloseMenu()
{
	PauseUI->SetVisibility(ESlateVisibility::Hidden);

	SetGameInputMode();

	//GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
}

void APPPlayerController::OpenPartsPauseWidget(TSubclassOf<UUserWidget> InWidgetClass, const TArray<TSoftObjectPtr<UTexture2D>>& InPartsInfo)
{
	SetPause(true);

	if (!IsValid(InWidgetClass))
	{
		return;
	}

	UUserWidget* PauseWidget = CreateWidget<UUserWidget>(this, InWidgetClass);
	if (PauseWidget)
	{
		if (IsValid(CurrentPauseWidget))
		{
			CurrentPauseWidget->RemoveFromViewport();
		}

		CurrentPauseWidget = PauseWidget;
		PauseWidget->AddToViewport(2);
		PauseWidget->SetVisibility(ESlateVisibility::Visible);

		UPPPartsPauseUIBase* PartsUIBase = Cast<UPPPartsPauseUIBase>(PauseWidget);
		if (PartsUIBase)
		{
			PartsUIBase->EndUIDelegate.BindUObject(this, &APPPlayerController::ClosePartsPauseWidget);
			PartsUIBase->SetPartsInfoImageArray(InPartsInfo);
		}

		SetUIInputMode(CurrentPauseWidget);
	}
}

void APPPlayerController::ClosePartsPauseWidget()
{
	if (IsValid(CurrentPauseWidget))
	{
		CurrentPauseWidget->RemoveFromViewport();

		SetGameInputMode();
	}
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	PauseUI = CreateWidget<UUserWidget>(this, PauseUIClass);
	UPPPauseMenyHUD* PauseMenu = Cast<UPPPauseMenyHUD>(PauseUI);
	if (PauseMenu)
	{
		PauseMenu->AddToViewport(1);
		PauseMenu->bIsFocusable = true;
		CloseMenu();
	}
	//SetInputMode(FInputModeGameAndUI());

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("LocalController"));
		if (InGameUIMainClass)
		{
			UE_LOG(LogTemp, Log, TEXT("InGameUIClass"));
			InGameUIMain = CreateWidget<UPPInGameUIMain>(this, InGameUIMainClass);
			InGameUIMain->SetOwningActor(GetPawn());
			if (InGameUIMain)
			{
				InGameUIMain->AddToViewport();	// ȭ�鿡 ǥ��
			}
		}

	}

	APPPlayerState* PS = GetPlayerState<APPPlayerState>();
	if (IsValid(PS))
	{
		PS->SetOwnerPlayerController(this);
	}
}

void APPPlayerController::SetUIInputMode(TObjectPtr<class UUserWidget> FocusTarget)
{
	/*FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(FocusTarget->TakeWidget());
	SetInputMode(InputMode);*/

	FInputModeUIOnly InputUIOnly;
	InputUIOnly.SetWidgetToFocus(FocusTarget->TakeWidget());
	SetInputMode(InputUIOnly);

	bShowMouseCursor = true;
}

void APPPlayerController::SetGameInputMode()
{
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	bShowMouseCursor = false;

	SetPause(false);
}
