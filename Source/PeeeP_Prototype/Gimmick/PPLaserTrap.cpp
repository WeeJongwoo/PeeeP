// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPLaserTrap.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"

// Sets default values
APPLaserTrap::APPLaserTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
	LaserMesh->SetCollisionProfileName(TEXT("NoCollision"));

	LaserCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserCollider"));
	LaserCollider->SetCollisionProfileName(TEXT("LaserTrap"));

	RootComponent = LaserMesh;
	LaserCollider->SetupAttachment(LaserMesh);

	TrapDamage = 5.0f;
}

// Called when the game starts or when spawned
void APPLaserTrap::BeginPlay()
{
	Super::BeginPlay();
	
	LaserCollider->OnComponentBeginOverlap.AddDynamic(this, &APPLaserTrap::OnLaserOverlap);
}

// Called every frame
void APPLaserTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPLaserTrap::OnLaserOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor);
	if (IsValid(Player))
	{
		Player->TakeDamage(TrapDamage);
	}
}

