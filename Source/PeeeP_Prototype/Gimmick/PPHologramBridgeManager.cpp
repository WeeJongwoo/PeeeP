// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPHologramBridgeManager.h"
#include "InteractionObject/PPButton.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequence.h"
#include "Gimmick/PPHologramBridge.h"

// Sets default values
APPHologramBridgeManager::APPHologramBridgeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bButton1State = false;
	bButton2State = false;
	bIsClear = false;
	bSequencePlayed = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
}

// Called when the game starts or when spawned
void APPHologramBridgeManager::BeginPlay()
{
	Super::BeginPlay();

}

void APPHologramBridgeManager::SetClear()
{
	if (bButton1State && bButton2State)
	{
		bIsClear = true;
		if (IsValid(ClearSequencePlayer) && !bSequencePlayed)
		{
			ClearSequencePlayer->Play();
			SequenceFinished();
			if (IsValid(HologramBridge))
			{
				HologramBridge->OnBridge();
			}
		}
	}
}

void APPHologramBridgeManager::SequenceFinished()
{
	bSequencePlayed = true;
}

// Called every frame
void APPHologramBridgeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPHologramBridgeManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(Button1))
	{
		Button1->OnButtonPressedDelegate.BindLambda([this]()
			{
				if (IsValid(this))
				{
					bButton1State = true;
					SetClear();
					UE_LOG(LogTemp, Log, TEXT("Button1 Pressed"));
				}
			}
		);
	}

	if (IsValid(Button2))
	{
		Button2->OnButtonPressedDelegate.BindLambda([this]()
			{
				if (IsValid(this))
				{
					bButton2State = true;
					SetClear();
					UE_LOG(LogTemp, Log, TEXT("Button2 Pressed"));
				}
			}
		);
	}

	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;
	Settings.bDisableMovementInput = true;
	Settings.bDisableLookAtInput = true;
	Settings.bHidePlayer = false;
	Settings.bHideHud = true;

	ALevelSequenceActor* RestartSequenceActor;

	ClearSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), ClearSequence, Settings, RestartSequenceActor);
}

