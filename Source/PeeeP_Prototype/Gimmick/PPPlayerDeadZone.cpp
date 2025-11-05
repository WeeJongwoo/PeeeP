// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPPlayerDeadZone.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"

// Sets default values
APPPlayerDeadZone::APPPlayerDeadZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

}

// Called when the game starts or when spawned
void APPPlayerDeadZone::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPlayerDeadZone::OnOverlapBegin);
}

void APPPlayerDeadZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor);
	if (IsValid(Player))
	{
		Player->TakeDamage(9999, false);
	}
}

// Called every frame
void APPPlayerDeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

