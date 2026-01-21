// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPInventorySaveTypes.generated.h"

/**
 * 
 */
class PEEEP_PROTOTYPE_API PPInventorySaveTypes
{
public:
	PPInventorySaveTypes();
	~PPInventorySaveTypes();
};

USTRUCT(BlueprintType)
struct FPPInventoryPartSaveData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(SaveGame)
	FName ItemID;

	UPROPERTY(SaveGame)
	int32 Quantity;
};