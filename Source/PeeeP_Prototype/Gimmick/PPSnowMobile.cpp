#include "Gimmick/PPSnowMobile.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/Character.h"

APPSnowMobile::APPSnowMobile()
{
	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	SetRootComponent(PathSpline);
	PathSpline->bEditableWhenInherited = true;

	SnowmobileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnowmobileMesh"));
	SnowmobileMesh->SetupAttachment(PathSpline);
	SnowmobileMesh->SetGenerateOverlapEvents(true);
	SnowmobileMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SnowmobileMesh->OnComponentBeginOverlap.AddDynamic(this, &APPSnowMobile::OnSnowmobileOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void APPSnowMobile::BeginPlay()
{
	Super::BeginPlay();
}

void APPSnowMobile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPaused) return;

	const float SplineLength = PathSpline->GetSplineLength();
	if (SplineLength <= 0.f) return;

	DistanceAlongSpline += MovementSpeed * DeltaTime;

	if (bLoopPath)
	{
		DistanceAlongSpline = FMath::Fmod(DistanceAlongSpline, SplineLength);
		if (DistanceAlongSpline < 0.f) DistanceAlongSpline += SplineLength;
	}
	else
	{
		DistanceAlongSpline = FMath::Clamp(DistanceAlongSpline, 0.f, SplineLength);
	}

	const FVector NewLoc = PathSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	const FVector Dir = PathSpline->GetTangentAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World).GetSafeNormal();
	LastDirection = Dir;
	FRotator NewRot = Dir.Rotation() + MeshRotationOffset;

	SnowmobileMesh->SetWorldLocationAndRotation(NewLoc, NewRot);
}

void APPSnowMobile::OnSnowmobileOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool,
	const FHitResult&)
{
	if (APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor))
	{
		Player->TakeDamage(DamageAmount, true);
		if (ACharacter* Char = Cast<ACharacter>(Player))
		{
			Char->LaunchCharacter(LastDirection * KnockbackStrength, true, true);
		}
	}

	/*if (!bIsPaused)
	{
		bIsPaused = true;
		GetWorldTimerManager().SetTimer(PauseTimerHandle, this, &APPSnowMobile::ResumeMovement, StopDuration, false);
	}*/
}

void APPSnowMobile::ResumeMovement()
{
	bIsPaused = false;
}
