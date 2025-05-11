// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricMovingPlatform.h"
#include "Components/InterpToMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
APP_ElectricMovingPlatform::APP_ElectricMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메쉬 컴포넌트 지정
	// 움직이는 플랫폼의 경우 중력의 영향을 받지 않으므로 SetSimulatePhysics를 false로 설정해야 함.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);								// 물리의 영향을 받지 않는다면 반드시 false로 설정.
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// 플레이어의 전기 방출을 받기 위한 콜리전 프로필 세팅.

	// InterpToMovement 컴포넌트 부분
	InterpToMovement = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovement"));
	InterpToMovement->SetUpdatedComponent(RootComponent);
	InterpToMovement->SetComponentTickEnabled(true);
	StartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Start Position"));
	//StartPosition->SetupAttachment(RootComponent);
	EndPosition = CreateDefaultSubobject<USceneComponent>(TEXT("End Position")); 
	//EndPosition->SetupAttachment(RootComponent);

	// InterpToMovement 컴포넌트 세팅 부분
	// 나중에 경로 지점을 추가할 때는 코드에서 추가하는 것이 아닌 BP에서 추가하는 방법이 더 편할 듯
	// 코드에서 지정 시 재활용이 어려움
	bIsCharged = false;													// 중복으로 켜지는 것을 방지하기 위한 flag입니다. 추후 발판 작동 시간 등 조절하려면 상황에 맞게 수정해 주세요.
	InterpToMovement->BehaviourType = EInterpToBehaviourType::PingPong;	// 한번만 이동하는 방식으로 설정. (Looping, OneShot, PingPong)
	InterpToMovement->bAutoActivate = false;							// 자동 시작 비활성화
	InterpToMovement->bSweep = false;									// 물체 충돌 시 경로 결정 방법
	StopTime = 2.0f;													// 도착 시 정지하는 시간

	StopTimerHandle.Invalidate();											// 타이머 핸들 초기화
	// 플랫폼 이동 시간, 이동 경로는 BP에서 설정해 주세요.
}

// Called when the game starts or when spawned
void APP_ElectricMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (InterpToMovement)
	{
		InterpToMovement->OnInterpToReverse.AddDynamic(this, &APP_ElectricMovingPlatform::OnReverse);
	}
}

void APP_ElectricMovingPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(LogTemp, Warning, TEXT("OnConstruction called!"));

	// InterpToMovement 컴포넌트의 ControlPoints를 설정합니다.
	if (InterpToMovement)
	{
		InterpToMovement->ControlPoints.Empty();	// 기존의 ControlPoints를 비웁니다.	
		InterpToMovement->AddControlPointPosition(StartPosition->GetComponentLocation(), false);
		InterpToMovement->AddControlPointPosition(EndPosition->GetComponentLocation(), false);
	}
}

void APP_ElectricMovingPlatform::Charge()
{
	if (bIsCharged)	// 이미 차징이 되었으므로 중복은 필요 없음
	{
		UE_LOG(LogTemp, Log, TEXT("This Platform is already Charged!"));
		return;
	}

	if (NewMaterial)
	{
		Mesh->SetMaterial(1, NewMaterial);
	}

	UE_LOG(LogTemp, Log, TEXT("Platform Charged!"));
	bIsCharged = true;

	if (InterpToMovement)
	{
		InterpToMovement->Activate();
	}
}

void APP_ElectricMovingPlatform::MoveBackToStart()
{
	if (InterpToMovement)
	{
		InterpToMovement->SetComponentTickEnabled(true);
	}
}

void APP_ElectricMovingPlatform::OnReverse(const FHitResult& ImpactResult, float Time)
{
	if (InterpToMovement)
	{
		InterpToMovement->SetComponentTickEnabled(false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnReverse called!"));
	StopTimerHandle.Invalidate();	// 타이머 핸들 초기화
	GetWorldTimerManager().SetTimer(StopTimerHandle, this, &APP_ElectricMovingPlatform::MoveBackToStart, StopTime, false);
}

// Called every frame
void APP_ElectricMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

