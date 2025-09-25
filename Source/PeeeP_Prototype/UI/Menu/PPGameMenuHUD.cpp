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
#include "GameMode/PPLevelLoadGIS.h"
#include "GameMode/PPSaveGame.h"
#include "GameMode/PPSaveGameSubsystem.h"

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

		// 새로운 게임을 시작할 때 이전 저장 데이터를 초기화
		if (UPPSaveGameSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UPPSaveGameSubsystem>())
		{
			SaveSubsystem->LastLoadedSaveData = nullptr; 
		}

		UPPLevelLoadGIS* LevelLoadGIS = GameInstance->GetSubsystem<UPPLevelLoadGIS>();
		if (LevelLoadGIS)
		{
			if (!StartLevel.IsNull())
			{
				LevelLoadGIS->LoadLevel(StartLevel);
			}
			else
			{
				UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage1"));
			}
		}
	}
}

void UPPGameMenuHUD::LoadButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("LoadOpen"));

	// 세이브 기능을 이용하여 로드(테스트)
	if (UPPSaveGame* SaveData = UPPSaveGame::LoadSaveData(this, TEXT("0"), 0))
	{

		UE_LOG(LogTemp, Log, TEXT("Loaded TestValue: %s"), *SaveData->TestValue);
		UE_LOG(LogTemp, Log, TEXT("Loaded PlayerLocation: %s"), *SaveData->PlayerLocation.ToString());
		UE_LOG(LogTemp, Log, TEXT("Loaded PlayerRotation: %s"), *SaveData->PlayerRotation.ToString());
		UE_LOG(LogTemp, Log, TEXT("Loaded LevelName: %s"), *SaveData->LevelName);

		// 저장된 데이터를 바탕으로 인게임으로 넘어가는 부분
		UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameInstance)
		{
			//GameInstance->SetLoadPlayerLocation(SaveData->PlayerLocation);
			//GameInstance->SetLoadPlayerRotation(SaveData->PlayerRotation);
			UPPLevelLoadGIS* LevelLoadGIS = GameInstance->GetSubsystem<UPPLevelLoadGIS>();
			if (LevelLoadGIS)
			{
				// 저장된 레벨로 이동
				TSoftObjectPtr<UWorld> LoadLevel = TSoftObjectPtr<UWorld>(FSoftObjectPath(FString::Printf(TEXT("/Game/PeeeP_Level/%s.%s"), *SaveData->LevelName, *SaveData->LevelName)));
				if(!LoadLevel.IsNull())
				{
					LevelLoadGIS->LoadLevel(LoadLevel);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to create LoadLevel from SaveData LevelName"));
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveData"));
	}
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
