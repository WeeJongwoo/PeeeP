// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPGameInstance.h"

void UPPGameInstance::SetInventoryPartsArray(TMap<int32, TPair<FName, int32>> NewInventoryPartsArray)
{
	InventoryPartsArray = NewInventoryPartsArray;
}

void UPPGameInstance::SetCurrentSlotIndex(int32 NewCurrentSlotIndex)
{
	CurrentSlotIndex = NewCurrentSlotIndex;
}
