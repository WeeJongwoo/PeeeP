// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/PPSaveGameBase.h"
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
	UPROPERTY()
	FString TestValue;

	// Player Location
	UPROPERTY()
	FVector PlayerLocation;
	// Player Rotation
	UPROPERTY()
	FRotator PlayerRotation;
	// Level Name
	UPROPERTY()
	FString LevelName;
};
