// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPTrainSpline.h"
#include "Components/SplineComponent.h"
#include "PPTrain.h"
#include "PPCrossline.h"

// Sets default values
APPTrainSpline::APPTrainSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(Root);
	
	bIsLoop = false; // 반복 여부

	MoveSpeed = 0.0f;

	CurrentDistance = 0.0f;
	DecelerationRate = 0.0f;
	bPendingStop = false;
	bStopTrain = false;
	bIsStopping = false;

	TotalMoveTime = SplineComponent->Duration;
}

// Called when the game starts or when spawned
void APPTrainSpline::BeginPlay()
{
	Super::BeginPlay();
	
	if (ActorToMoveClass == nullptr)	return;

	ActorToMove = GetWorld()->SpawnActor<AActor>(ActorToMoveClass, SplineComponent->GetComponentTransform());
	
	if (ActorToMove != nullptr)
	{
		// 시작 시간 저장  GetTimeSeconds() : 게임을 시작한 후 현재까지 경과된 시간을 리턴함.
		StartTime = GetWorld()->GetTimeSeconds();
		//액터 이동 가능 여부
		bCanMoveActor = true;
		//참일 경우 정방향으로 이동, 거짓일 경우 역방향으로 이동.
		bIsMovingForward = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Spline Length : %f"), SplineComponent->GetSplineLength());

}

void APPTrainSpline::SetMoveActorRotationAndLocation(float Distance)
{
	//Spline 선에 (Tick에서 계속 업데이트 되고 있는) Distance값을 기준으로 위치,회전값을 얻어온다.   
	FVector Position = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
	//얻어온 값들에 플레이어를 위치시킨다.
	USkeletalMeshComponent* ActorMesh = ActorToMove->FindComponentByClass<USkeletalMeshComponent>();
	if (ActorMesh != nullptr)
	{
		ActorMesh->SetWorldLocation(Position);
		ActorMesh->SetWorldRotation(Rotator);
	}
}

void APPTrainSpline::SetMoveActorLocation(float Distance)
{
	FVector Position = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	USkeletalMeshComponent* ActorMesh = ActorToMove->FindComponentByClass<USkeletalMeshComponent>();
	if (ActorMesh != nullptr)
	{
		ActorMesh->SetWorldLocation(Position);
	}
}

void APPTrainSpline::SetMoveActorRotation(float Distance)
{
	FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
	USkeletalMeshComponent* ActorMesh = ActorToMove->FindComponentByClass<USkeletalMeshComponent>();
	if (ActorMesh != nullptr)
	{
		ActorMesh->SetWorldRotation(Rotator);
	}
}

// Called every frame
void APPTrainSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((ActorToMove == nullptr) || !bCanMoveActor) return;

	float SplineLength = SplineComponent->GetSplineLength();
	CurrentDistance = FMath::Clamp(CurrentDistance, 0.0f, SplineLength);
	
	CurrentDistance += MoveSpeed * DeltaTime;
	
	if (bIsMovingForward)
	{
		if (CurrentDistance >= SplineComponent->GetSplineLength())
		{
			UE_LOG(LogTemp, Warning, TEXT("End Point!"));
			if (bIsLoop)
			{
				UE_LOG(LogTemp, Warning, TEXT("Loop!"));
				CurrentDistance = 0.0f;
				
				if (bPendingStop)
				{
					bStopTrain = true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("End!"));
				bCanMoveActor = false;
			}
		}

		// 정지 신호 후 경로 끝에 도달(정방향)
		if (bStopTrain)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Distance: %f"), CurrentDistance);


			if (CurrentDistance >= StopDistance)
			{
				if (!bIsStopping)
				{
					bIsStopping = true;
					APPTrain* TrainActor = Cast<APPTrain>(ActorToMove);
					if (TrainActor)
					{
						UE_LOG(LogTemp, Warning, TEXT("Train Actor Found!"));
						TrainActor->StopTrain();
					}
				}

				MoveSpeed = FMath::FInterpTo(MoveSpeed, 0.0f, DeltaTime, DecelerationRate);
				// 정지 보정 및 정지 후 처리 부분
				if (MoveSpeed <= 10.0f)
				{
					bPendingStop = false;
					bStopTrain = false;
					bCanMoveActor = false;
					bIsStopping = false;
					MoveSpeed = 0.0f;

					if (!CrosslineActors.IsEmpty())
					{
						for (APPCrossline* Crossline : CrosslineActors)
						{
							if (Crossline)
							{
								Crossline->OnCrosslineEventDelegate.ExecuteIfBound(false);
							}
						}
					}
				}
			}
		}

		SetMoveActorLocation(CurrentDistance);
	}
}

void APPTrainSpline::StopActorMoveEaseIn()
{
	bPendingStop = true;
}

void APPTrainSpline::TrafficLightEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Traffic Light Event Triggered!"));
	StopActorMoveEaseIn();
}

