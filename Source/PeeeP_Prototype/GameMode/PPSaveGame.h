// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/PPSaveGameBase.h"
#include "Inventory/PPInventorySaveTypes.h"
#include "PPSaveGame.generated.h"

/**
 * 
 */

UCLASS()
class PEEEP_PROTOTYPE_API UPPSaveGame : public UPPSaveGameBase
{
	GENERATED_BODY()
	
public:
	static UPPSaveGame* LoadSaveData(UObject* inObj, const FString& inUniqueID, int32 inUserIndex);
public:
	// 저장할 데이터
	
	// Test
	UPROPERTY(SaveGame)
	FString TestValue;

	// Player Location
	UPROPERTY(SaveGame)
	FVector PlayerLocation;
	// Player Rotation
	UPROPERTY(SaveGame)
	FRotator PlayerRotation;
	// Level Name
	UPROPERTY(SaveGame)
	FString LevelName;

	// Inventory Parts
	UPROPERTY(SaveGame)
	TMap<int32, FPPInventoryPartSaveData> InventoryPartsArray;
	UPROPERTY(SaveGame)
	int32 CurrentSlotIndex;

	// Battery
	UPROPERTY(SaveGame)
	float PlayerElectricCapacity;
};
