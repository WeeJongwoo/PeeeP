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

	float CurrentElectricCapacity;

	UPROPERTY()
	TObjectPtr<class USoundMix> MasterSoundMix;

public:
	void SetInventoryPartsArray(TMap<int32, TPair<FName, int32>> NewInventoryPartsArray);
	// 인벤토리 불러오기
	FORCEINLINE TMap<int32, TPair<FName, int32>> GetInventoryPartsArray() { return InventoryPartsArray; }

	void SetCurrentSlotIndex(int32 NewCurrentSlotIndex);
	FORCEINLINE int32 GetCurrentSlotIndex() { return CurrentSlotIndex; }

	void ClearInventoryPartsArray();

	void SetCurrentElectricCapacity(float NewCurrentElectricCapacity);
	FORCEINLINE float GetCurrentElectricCapacity() { return CurrentElectricCapacity; }

	// Audio
	float MasterVolume;
	float MusicVolume;
	float SFXVolume;

	void ApplySavedAudioSettings();

	// Control
	float MouseSensitivity;

	void ApplySavedControlSettings();

	UDataTable* GetUIStringTable();

	int8 bWasLoadedFromSave : 1;

private:
	class UDataTable* UIStringTable;
};
