// PPTurbine.cpp
#include "Gimmick/PPTurbine.h"
#include "Components/StaticMeshComponent.h"

APPTurbine::APPTurbine()
{
	PrimaryActorTick.bCanEverTick = true;

	TurbineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurbineMesh"));
	RootComponent = TurbineMesh;
}

void APPTurbine::BeginPlay()
{
	Super::BeginPlay();
}

void APPTurbine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!RotationAxis.IsNearlyZero() && RotationSpeed > 0.f)
	{
		const FVector AxisNorm = RotationAxis.GetSafeNormal();
		const float AngleRad = FMath::DegreesToRadians(RotationSpeed * DeltaTime);
		const FQuat  DeltaQuat = FQuat(AxisNorm, AngleRad);

		AddActorLocalRotation(DeltaQuat, false, nullptr, ETeleportType::None);
	}
}
