// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PPSaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPSaveGameBase : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Iinitialize the save game object
	void InitializeSaveGameBase(const FString& inSlotName, int32 inUserIndex);

	// Save
	void SaveData();

protected:
	// 식별 정보
	FString SlotName;
	int32 UserIndex;
};
