// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPTrain.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Animation/AnimSingleNodeInstance.h"

// Sets default values
APPTrain::APPTrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TrainSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TrainSkeletalMesh"));
	TrainSkeletalMesh->SetupAttachment(Root);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(TrainSkeletalMesh);
	AudioComponent->bAutoActivate = false;

}

// Called when the game starts or when spawned
void APPTrain::BeginPlay()
{
	Super::BeginPlay();

	if (MoveSound)
	{
		AudioComponent->SetSound(MoveSound);
		AudioComponent->Play(0.0f);
	}
}

// Called every frame
void APPTrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APPTrain::StopTrain()
{
	StopAnimaionSoftly();
	if (HonkSound)
	{
		AudioComponent->FadeOut(7.5f, 0.0f, EAudioFaderCurve::Linear);
		UGameplayStatics::PlaySoundAtLocation(this, HonkSound, GetActorLocation());
	}
}

void APPTrain::StopAnimaionSoftly()
{
	UAnimSingleNodeInstance* AnimInstance = TrainSkeletalMesh->GetSingleNodeInstance();
	if (AnimInstance)
	{
		AnimInstance->SetPlayRate(0.0f);
	}
}

