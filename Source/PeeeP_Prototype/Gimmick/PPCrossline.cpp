// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPCrossline.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
APPCrossline::APPCrossline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CrosslinePillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrosslinePillarMesh"));
	CrosslinePillarMesh->SetupAttachment(SceneRoot);
	CrosslinePillarMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	CrosslineHoloMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrosslineHoloMesh"));
	CrosslineHoloMesh->SetupAttachment(SceneRoot);
	CrosslineHoloMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CrosslineCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CrosslineCollisionBox"));
	CrosslineCollisionBox->SetupAttachment(SceneRoot);
	CrosslineCollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(SceneRoot);
	AudioComponent->bAutoActivate = true;

}

// Called when the game starts or when spawned
void APPCrossline::BeginPlay()
{
	Super::BeginPlay();

	OnCrosslineEventDelegate.BindUObject(this, &APPCrossline::SetCrosslineMeshVisibility);

	if (CrosslineSound)
	{
		AudioComponent->SetSound(CrosslineSound);
		AudioComponent->Play();
	}
}

// Called every frame
void APPCrossline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPCrossline::SetCrosslineMeshVisibility(bool bVisible)
{
	{
		CrosslineHoloMesh->SetVisibility(bVisible);
		if (bVisible)
		{
			CrosslineCollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
			AudioComponent->Play();
		}
		else
		{
			CrosslineCollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
			AudioComponent->SetSound(CrosslineOffSound);
			AudioComponent->Play();
		}
	}
}

