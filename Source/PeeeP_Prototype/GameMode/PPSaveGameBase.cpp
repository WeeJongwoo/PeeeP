// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPSaveGameBase.h"
#include "Kismet/GameplayStatics.h"

void UPPSaveGameBase::InitializeSaveGameBase(const FString& inSlotName, int32 inUserIndex)
{
	SlotName = inSlotName;
	UserIndex = inUserIndex;
}

void UPPSaveGameBase::SaveDate()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex);
}
