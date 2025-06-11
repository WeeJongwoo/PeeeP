// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerState.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequence.h"
#include "MovieSceneBindingOverrides.h"
#include "Interface/PPSavepointInterface.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPPlayerController.h"


APPPlayerState::APPPlayerState()
{
	
}

void APPPlayerState::SetSpawnActorLocation(AActor* InActor)
{
	SpawnPointActor = InActor;

	SpawnLocation = InActor->GetActorLocation();

	UDefaultLevelSequenceInstanceData* DefaultInstanceData = NewObject<UDefaultLevelSequenceInstanceData>();
	DefaultInstanceData->TransformOriginActor = InActor;
	if (IsValid(RestartSequenceActor))
	{
		RestartSequenceActor->DefaultInstanceData = DefaultInstanceData;
	}
}

void APPPlayerState::SetOwnerPlayerController(APPPlayerController* InPlayerController)
{
	if (IsValid(InPlayerController))
	{
		OwnerPlayerController = InPlayerController;
	}
}

void APPPlayerState::PlayRespawnSequence()
{
	if (IsValid(SpawnPointActor) && SpawnPointActor->Implements<UPPSavepointInterface>())
	{
		if (IsValid(RestartSequencePlayer))
		{
			UE_LOG(LogTemp, Log, TEXT("Play Sequence"));
			RestartSequencePlayer->Play();

			if (IsValid(OwnerPlayerController))
			{
				APawn* PossessPawn = OwnerPlayerController->GetPawn();
				if (IsValid(PossessPawn))
				{
					PossessPawn->DisableInput(OwnerPlayerController);
				}
			}
		}
	}
	else
	{
		ReturnInput();
	}
}

void APPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	static const FString RestartSequencePath = TEXT("/Game/PeeeP_Sequence/LS_Respawn.LS_Respawn");
	RestartSequence = LoadObject<ULevelSequence>(nullptr, *RestartSequencePath);

	if (IsValid(RestartSequence))
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;
		Settings.bDisableMovementInput = true;
		Settings.bDisableLookAtInput = true;
		Settings.bHidePlayer = false;
		Settings.bHideHud = true;

		RestartSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), RestartSequence, Settings, RestartSequenceActor);

		RestartSequenceActor->SetBindingByTag(FName("Player"), { GetPawn() });
		RestartSequenceActor->bOverrideInstanceData = true;

		RestartSequencePlayer->OnPlay.AddDynamic(this, &APPPlayerState::StartRespawnSequence);
		RestartSequencePlayer->OnFinished.AddDynamic(this, &APPPlayerState::ReturnInput);
		
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);
		if (FoundActors.Num()>0)
		{
			RestartSequenceActor->SetBindingByTag(FName("PostProcessVolume"), { FoundActors[0] });
		}

		UE_LOG(LogTemp, Warning, TEXT("LevelSequnceSetting"));
	}
}

void APPPlayerState::ReturnInput()
{
	if (IsValid(OwnerPlayerController))
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (IsValid(PlayerPawn))
		{
			PlayerPawn->EnableInput(OwnerPlayerController);
		}
	}
}

void APPPlayerState::StartRespawnSequence()
{
	if (IsValid(OwnerPlayerController))
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			PlayerPawn->DisableInput(OwnerPlayerController);
		}
	}
}
