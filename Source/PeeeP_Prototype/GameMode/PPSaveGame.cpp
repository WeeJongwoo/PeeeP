// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPSaveGame.h"
#include "GameMode/PPSaveGameSubsystem.h"



UPPSaveGame* UPPSaveGame::LoadSaveData(UObject* inObj, const FString& inUniqueID, int32 inUserIndex)
{
	return UPPSaveGameSubsystem::LoadSaveData<UPPSaveGame>(inObj, inUniqueID, inUserIndex);

	// 사용 예시
	//bool UTestUserWidget::TestSaveLogic()
	//{
	//	if (UTestSaveGame* saveData = UTestSaveGame::LoadData(this, TEXT("0"), 0))
	//	{
	//		saveData->TestValue = TEXT("Test");
	//		saveData->SaveData();
	//		return true;
	//	}

	//	return false;
	//}
}