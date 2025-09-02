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
	
	UPPGameInstance();


protected:
	TMap<int32, TPair<FName, int32>> InventoryPartsArray;
	int32 CurrentSlotIndex;

	UPROPERTY()
	TObjectPtr<class USoundMix> MasterSoundMix;

public:
	void SetInventoryPartsArray(TMap<int32, TPair<FName, int32>> NewInventoryPartsArray);
	FORCEINLINE TMap<int32, TPair<FName, int32>> GetInventoryPartsArray() { return InventoryPartsArray; }

	void SetCurrentSlotIndex(int32 NewCurrentSlotIndex);
	FORCEINLINE int32 GetCurrentSlotIndex() { return CurrentSlotIndex; }

	void ClearInventoryPartsArray();

	// Audio
	float MasterVolume;
	float MusicVolume;
	float SFXVolume;

	void ApplySavedAudioSettings();

	// Control
	float MouseSensitivity;

	void ApplySavedControlSettings();

	UDataTable* GetUIStringTable();

private:
	class UDataTable* UIStringTable;
};
