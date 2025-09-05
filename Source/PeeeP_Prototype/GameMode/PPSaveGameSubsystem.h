// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPSaveGameBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PPSaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Load Save File
	template<typename T>
	static T* LoadSaveData(UObject* inObj, const FString& inUniqueID, int32 inUserIndex)
	{
		if (UPPSaveGameSubsystem* Subsytem = GetSaveGameSubsystem(inObj))
		{
			return Cast<T>(Subsytem->LoadDataLogic(T::StaticClass(), inUniqueID, inUserIndex));
		}
		return nullptr;
	}

protected:
	// Get Save Game Subsystem
	static UPPSaveGameSubsystem* GetSaveGameSubsystem(UObject* inObj);

	// Data Load Logic
	UPPSaveGameBase* LoadDataLogic(UClass* inClass, const FString& inUniqueID, int32 inUserIndex);

public:
	// Prefix Map
	UPROPERTY()
	TMap<UClass*, FString> SlotNamePrefixMap;
};
