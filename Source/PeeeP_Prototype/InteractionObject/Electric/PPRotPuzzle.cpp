// Fill out your copyright notice in the Description page of Project Settings.


#include "PPRotPuzzle.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APPRotPuzzle::APPRotPuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotationDirection = EPPRotateDirection::CCW;
	RotationTime = 1.0f;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	RootComponent = Frame;

	Wing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing"));
	Wing->SetupAttachment(Frame);
}

// Called when the game starts or when spawned
void APPRotPuzzle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPRotPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		CurrentRotatingTime += DeltaTime;
		float Alpha = FMath::Clamp(CurrentRotatingTime / RotationTime, 0.0f, 1.0f);

		FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
		Wing->SetRelativeRotation(NewRotation);

		if (Alpha >= 1.0f)
		{
			bIsRotating = false;
			
			UStaticMesh* FrameMesh = Cast<UStaticMesh>(Frame->GetStaticMesh());
			if (FrameMesh && FrameMesh->GetStaticMaterials().IsValidIndex(1)) //메시 가져와서 머테리얼 인덱스 1번 슬롯 존재하는지 확인.
			{
				if (IsValid(DeactivedMaterial))
				{
					Frame->SetMaterial(1, DeactivedMaterial);
				}
			}
		}
	}
}

void APPRotPuzzle::Charge()
{
	if (bIsRotating)
	{
		return;
	}

	bIsRotating = true;
	StartRotation = Wing->GetRelativeRotation();
	CurrentRotatingTime = 0.0f;

	if (RotationDirection == EPPRotateDirection::CCW)
	{
		TargetRotation = StartRotation + FRotator(0.0f, -90.0f, 0.0f);
	}
	else
	{
		TargetRotation = StartRotation + FRotator(0.0f, 90.0f, 0.0f);
	}
	
	UStaticMesh* FrameMesh = Cast<UStaticMesh>(Frame->GetStaticMesh());
	if (FrameMesh && FrameMesh->GetStaticMaterials().IsValidIndex(1))
	{
		if (IsValid(ActivedMaterial))
		{
			Frame->SetMaterial(1, ActivedMaterial);
		}
	}
}

