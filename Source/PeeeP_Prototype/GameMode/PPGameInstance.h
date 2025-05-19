// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	TMap<int32, TPair<FName, int32>> InventoryPartsArray;
	int32 CurrentSlotIndex;

public:
	void SetInventoryPartsArray(TMap<int32, TPair<FName, int32>> NewInventoryPartsArray);
	FORCEINLINE TMap<int32, TPair<FName, int32>> GetInventoryPartsArray() { return InventoryPartsArray; }

	void SetCurrentSlotIndex(int32 NewCurrentSlotIndex);
	FORCEINLINE int32 GetCurrentSlotIndex() { return CurrentSlotIndex; }

	void ClearInventoryPartsArray();
};
