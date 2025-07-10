// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPGameInstance.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"


UPPGameInstance::UPPGameInstance()
{
	// Initialize the MasterSoundMix if it is not set
	static ConstructorHelpers::FObjectFinder<USoundMix> MasterSoundMixRef(TEXT("/Script/Engine.SoundMix'/Game/SFX/SCM_SoundMix.SCM_SoundMix'"));
	if (nullptr != MasterSoundMixRef.Object)
	{
		MasterSoundMix = MasterSoundMixRef.Object;
	}

	MasterVolume = 1.0f;
	MusicVolume = 1.0f;
	SFXVolume = 1.0f;
}

void UPPGameInstance::SetInventoryPartsArray(TMap<int32, TPair<FName, int32>> NewInventoryPartsArray)
{
	InventoryPartsArray = NewInventoryPartsArray;
}

void UPPGameInstance::SetCurrentSlotIndex(int32 NewCurrentSlotIndex)
{
	CurrentSlotIndex = NewCurrentSlotIndex;
}

void UPPGameInstance::ClearInventoryPartsArray()
{
	InventoryPartsArray.Empty();
	CurrentSlotIndex = 0;
}

void UPPGameInstance::ApplySavedAudioSettings()
{
	if (MasterSoundMix)
	{
		MasterSoundMix->SoundClassEffects[0].VolumeAdjuster = MasterVolume;
		MasterSoundMix->SoundClassEffects[1].VolumeAdjuster = MusicVolume;
		MasterSoundMix->SoundClassEffects[2].VolumeAdjuster = SFXVolume;

		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
	}
}
