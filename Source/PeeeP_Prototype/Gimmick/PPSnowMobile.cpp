#include "Gimmick/PPSnowMobile.h"
#include "Character/PPCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"

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

// BeginPlay
void APPSnowMobile::BeginPlay()
{
	Super::BeginPlay();

	if (PathSpline->GetNumberOfSplinePoints() < 2)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("PPSnowMobile(%s): ���ö��� ���� 2�� �̸��̶� �̵� �Ұ�!"),
			*GetName());
	}
}

void APPSnowMobile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	// ��ġ
	const FVector NewLoc = PathSpline->GetLocationAtDistanceAlongSpline(
		DistanceAlongSpline, ESplineCoordinateSpace::World);

	// ���� ����
	const FVector Dir = PathSpline->GetTangentAtDistanceAlongSpline(
		DistanceAlongSpline, ESplineCoordinateSpace::World).GetSafeNormal();

	FRotator NewRot = Dir.Rotation() + MeshRotationOffset;

	// �̵���ȸ��
	SnowmobileMesh->SetWorldLocationAndRotation(NewLoc, NewRot);
}

void APPSnowMobile::OnSnowmobileOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (APPCharacterPlayer* Player = Cast<APPCharacterPlayer>(OtherActor))
	{
		Player->TakeDamage(DamageAmount);
	}
}
