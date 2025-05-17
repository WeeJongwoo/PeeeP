// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPHologramBridgeManager.h"
#include "InteractionObject/Electric/PPButton.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequence.h"
#include "Gimmick/PPHologramBridge.h"
#include "Kismet/GameplayStatics.h"

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

	CameraBlendDuration = 0.5f;
	CameraViewDuration = 1.0f;
}

// Called when the game starts or when spawned
void APPHologramBridgeManager::BeginPlay()
{
	Super::BeginPlay();

}

void APPHologramBridgeManager::SetClear()
{
	if(bSequencePlayed)
	{
		return;
	}

	if (bButton1State && bButton2State)
	{
		bIsClear = true;
		if (IsValid(ClearSequencePlayer) && !bSequencePlayed)
		{
			ClearSequencePlayer->Play();
			SequenceFinished();
		}
	}
	else
	{
		SwitchToLightCam();
	}
}

void APPHologramBridgeManager::SequenceFinished()
{
	bSequencePlayed = true;
	if (IsValid(HologramBridge))
	{
		HologramBridge->OnBridge();
	}
}

void APPHologramBridgeManager::SwitchToLightCam()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DefaultCamActor = PlayerController->GetViewTarget();

	if (IsValid(DefaultCamActor) && IsValid(LightCamActor))
	{
		PlayerController->SetViewTargetWithBlend(LightCamActor, CameraBlendDuration);

		float ReturnCameraBlendTime = CameraViewDuration+CameraBlendDuration;

		GetWorld()->GetTimerManager().SetTimer(LightCamTimerHandle, this, &APPHologramBridgeManager::RevertCam, ReturnCameraBlendTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DefaultCamActor is not valid"));
	}
}

void APPHologramBridgeManager::RevertCam()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (IsValid(DefaultCamActor))
	{
		PlayerController->SetViewTargetWithBlend(DefaultCamActor, CameraBlendDuration);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DefaultCamActor is not valid"));
	}
}

void APPHologramBridgeManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(ClearSequencePlayer))
	{
		ClearSequencePlayer->Stop();
		ClearSequencePlayer->OnFinished.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
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
					if (!bButton1State)
					{
						bButton1State = true;
						SetClear();
						UE_LOG(LogTemp, Log, TEXT("Button1 Pressed"));
					}
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
					if (!bButton2State)
					{
						bButton2State = true;
						SetClear();
						UE_LOG(LogTemp, Log, TEXT("Button2 Pressed"));
					}
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
	if (ClearSequencePlayer)
	{
		ClearSequencePlayer->OnFinished.RemoveAll(this);
		ClearSequencePlayer->OnFinished.AddDynamic(this, &APPHologramBridgeManager::SequenceFinished);
	}
}

