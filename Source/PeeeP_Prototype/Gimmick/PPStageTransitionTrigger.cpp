﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPStageTransitionTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PPCharacterPlayer.h"
#include "Inventory/PPInventoryComponent.h"
#include "GameMode/PPGameInstance.h"
#include "GameMode/PPLevelLoadGIS.h"

// Sets default values
APPStageTransitionTrigger::APPStageTransitionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APPStageTransitionTrigger::OnTriggered);
}

// Called when the game starts or when spawned
void APPStageTransitionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPStageTransitionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPStageTransitionTrigger::OnTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		//UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(this));

		//if (GameInstance)
		//{
		//	//TArray<TObjectPtr<class UPPInventoryPartsItem>> PartsItems = PlayerCharacter->GetInventoryComponent()->GetPartsItems();
		//	//GameInstance->SetPartsItems(PartsItems);
		//}

		UPPLevelLoadGIS* LevelLoader = GetWorld()->GetGameInstance()->GetSubsystem<UPPLevelLoadGIS>();
		if (LevelLoader)
		{
			LevelLoader->LoadLevel(NextLevel);
		}

		//UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}

