// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPSaveGame.h"
#include "Character/PPCharacterPlayer.h"
#include "Component/PPElectricDischargeComponent.h"
#include "Inventory/PPInventoryComponent.h"

// Static function
bool UPPSaveGameSubsystem::SetSaveData(UPPSaveGame* SaveData, APPCharacterPlayer* InPlayer)
{
	if (SaveData == nullptr || InPlayer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PPSaveGameSubsystem] SaveData or InPlayer is null"));
		return false;
	}

	// TestValue String
	SaveData->TestValue = TEXT("PPSaveGameSubsystem Save");
	// Player Location
	SaveData->PlayerLocation = InPlayer->GetActorLocation();
	// Player Rotation
	SaveData->PlayerRotation = InPlayer->GetActorRotation();
	// Level Name
	SaveData->LevelName = UGameplayStatics::GetCurrentLevelName(InPlayer, true);
	// Inventory Parts
	SaveData->InventoryPartsArray = InPlayer->GetInventoryComponent()->GetSaveMap();
	SaveData->CurrentSlotIndex = InPlayer->GetInventoryComponent()->GetCurrentSlotIndex();
	// Battery
	SaveData->PlayerElectricCapacity = InPlayer->GetElectricDischargeComponent()->GetCurrentCapacity();

	UE_LOG(LogTemp, Warning, TEXT("[PPSaveGameSubsystem] Set Save Data Success"));

	return true;
}

void UPPSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 테스트
	// 슬롯 이름 프리픽스 등록
	SlotNamePrefixMap.Add(UPPSaveGame::StaticClass(), TEXT("UPPSaveGame"));
}

UPPSaveGameSubsystem* UPPSaveGameSubsystem::GetSaveGameSubsystem(UObject* inObj)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(inObj))
	{
		return GameInstance->GetSubsystem<UPPSaveGameSubsystem>();
	}

	return nullptr;
}

UPPSaveGameBase* UPPSaveGameSubsystem::LoadDataLogic(UClass* inClass, const FString& inUniqueID, int32 inUserIndex)
{
	FString* FindPrefix = SlotNamePrefixMap.Find(inClass);
	check(FindPrefix);

	// 중요: SlotName은 고유해야 함 (클래스명 + 유니크ID 조합)
	FString SlotName = FString::Printf(TEXT("%s_%s"), **FindPrefix, *inUniqueID);

	// 찾아보고 없으면 생성
	USaveGame* Temp = Cast<UPPSaveGameBase>(UGameplayStatics::LoadGameFromSlot(SlotName, inUserIndex));
	if (nullptr == Temp)
	{
		Temp = UGameplayStatics::CreateSaveGameObject(inClass);
	}

	UPPSaveGameBase* SaveData = Cast<UPPSaveGameBase>(Temp);
	if (nullptr == SaveData)
	{
		return nullptr;
	}

	// 초기화
	SaveData->InitializeSaveGameBase(SlotName, inUserIndex);
	return Cast<UPPSaveGameBase>(SaveData);
}
