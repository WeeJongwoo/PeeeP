// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "PPWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Level Settings")
	TObjectPtr<class UPPLevelData> LevelDataAsset;
	
public:
	const UPPLevelData* GetLevelDataAsset() const { return LevelDataAsset; }
};
