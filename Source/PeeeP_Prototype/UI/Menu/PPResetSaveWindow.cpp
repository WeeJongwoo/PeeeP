// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/PPResetSaveWindow.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPSaveGame.h"
#include "GameMode/PPSaveGameSubsystem.h"
#include "GameMode/PPGameInstance.h"
#include "GameMode/PPLevelLoadGIS.h"
#include "UI/Menu/PPGameMenuHUD.h"


void UPPResetSaveWindow::NativeConstruct()
{
	Super::NativeConstruct();
	if (ButtonYes)
	{
		ButtonYes->OnClicked.AddDynamic(this, &UPPResetSaveWindow::ButtonYesClick);
	}
	if (ButtonNo)
	{
		ButtonNo->OnClicked.AddDynamic(this, &UPPResetSaveWindow::ButtonNoClick);
	}
}

void UPPResetSaveWindow::ButtonYesClick()
{
	UE_LOG(LogTemp, Log, TEXT("[PPResetSaveWindon] Yes"));

	// 기존 세이브 파일 제거
	UGameplayStatics::DeleteGameInSlot(TEXT("UPPSaveGame_0"), 0);
	UE_LOG(LogTemp, Log, TEXT("[PPResetSaveWindow] Previous Save File Deleted Completely!"));

	UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		// 게임 인스턴스의 인벤토리 초기화
		GameInstance->ClearInventoryPartsArray();
		if (UPPSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPPSaveGameSubsystem>())
		{
			SaveSubsystem->LastLoadedSaveData = nullptr;
			UE_LOG(LogTemp, Log, TEXT("[PPResetSaveWindow] Data nullptr Set Completed."));
		}
	}

	OnResetSaveConfirmedDelegate.Broadcast();
	SetVisibility(ESlateVisibility::Hidden);
}

void UPPResetSaveWindow::ButtonNoClick()
{
	UE_LOG(LogTemp, Log, TEXT("[PPResetSaveWindon] No"));

	SetVisibility(ESlateVisibility::Hidden);
}
