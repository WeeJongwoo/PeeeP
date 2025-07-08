// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "PPLevelLoadGIS.generated.h"


/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPLevelLoadGIS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	void LoadLevel(const TSoftObjectPtr<class UWorld>& InTartgetLevel);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:

	TSharedPtr<FStreamableHandle> LevelHandle;

	void OnLoadingUpdate(TSharedRef<FStreamableHandle> Handle);

	// Callback function when the level is loaded
	void OnLevelLoaded();

	void OnPostLoadLevel(UWorld* LoadedWorld);

	void DoOpenLevel();

	TSoftObjectPtr<class UWorld> TargetLevel; // The level we want to load
	
	TSubclassOf<class UPPLoadingWidget> LoadingWidgetClass; // The class of the loading widget

	UPROPERTY()
	TObjectPtr<class UPPLoadingWidget> LoadingWidget; // The instance of the loading widget

	FTimerHandle LoadingTimerHandle; // Timer handle for managing loading time

	bool bIsLoading = false; // Flag to check if we are currently loading a level
	bool bMinTimeMet = false;

	void OnMinTimeReached();

	void TryToLoadLevel();

	void DeleteLoadingWidget();
};
