// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPSaveGame.h"

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
