// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LevelSequenceActor.h"
#include "PPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	APPPlayerState();

	void SetSpawnActorLocation(class AActor* InActor);
	FVector GetSpawnLocation() { return SpawnLocation; }

	void SetOwnerPlayerController(class APPPlayerController* InPlayerController);

	void PlayRespawnSequence();

private:

	void BeginPlay() override;

	UFUNCTION()
	void ReturnInput();

	UFUNCTION()
	void StartRespawnSequence();

	FVector SpawnLocation;
	TObjectPtr<class AActor> SpawnPointActor;

	TObjectPtr<class APPPlayerController> OwnerPlayerController;

	UPROPERTY()
	TObjectPtr<class ULevelSequence> RestartSequence;

	UPROPERTY()
	ALevelSequenceActor* RestartSequenceActor;

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> RestartSequencePlayer;

};
