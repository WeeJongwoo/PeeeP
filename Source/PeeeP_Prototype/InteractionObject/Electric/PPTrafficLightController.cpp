﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPTrafficLightController.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionObject/PPTrafficLightBase.h"
#include "InteractionObject/PPTrafficLightManager.h"
#include "Components/BoxComponent.h"
#include "InteractionObject/PPBattery.h"
#include "GameplayTagContainer.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPTrafficLightController::APPTrafficLightController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrafficLightControllerStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightControllerStaticMesh"));
	SetRootComponent(TrafficLightControllerStaticMesh);
	TrafficLightControllerStaticMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화
	TrafficLightControllerStaticMesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));	// 플레이어의 전기 방출을 받기 위한 콜리전 프로필 세팅.

	TrafficLightControllerMaterial = CreateDefaultSubobject<UMaterialInstance>(TEXT("TrafficLightControllerMaterial"));
	TrafficLightControllerStaticMesh->SetMaterial(0, TrafficLightControllerMaterial);

	TrafficLightControllerStateFrameStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightControllerStatePlaneStaticMesh"));
	TrafficLightControllerStateFrameStaticMesh->SetupAttachment(TrafficLightControllerStaticMesh);

	TrafficLightControllerScreenStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightControllerScreenStaticMesh"));
	TrafficLightControllerScreenStaticMesh->SetupAttachment(TrafficLightControllerStaticMesh);
	TrafficLightControllerScreenStaticMesh->SetSimulatePhysics(false);	// 물리를 사용하지 않으므로 비활성화
	TrafficLightControllerScreenStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));	// 콜리전 프로필을 NoCollision으로 설정

	TrafficLightControllerOffMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("TrafficLightControllerOffMaterial"));
	TrafficLightControllerOnMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("TrafficLightControllerOnMaterial"));
	TrafficLightControllerWorkingMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("TrafficLightControllerWorkingMaterial"));

	BatteryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BatteryTrigger"));
	BatteryTrigger->SetupAttachment(TrafficLightControllerStaticMesh);
	BatteryTrigger->OnComponentBeginOverlap.AddDynamic(this, &APPTrafficLightController::OnOverlapBegin);

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);

	ConstraintPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPoint"));
	ConstraintPoint->SetupAttachment(RootComponent);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetConstrainedComponents(nullptr, NAME_None, nullptr, NAME_None);
	PhysicsConstraint->SetDisableCollision(true);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	if (AudioComponent)
	{
		AudioComponent->SetupAttachment(RootComponent);
	}

	TrafficLightControllerState = ETrafficLightControllerState::TLC_OFF;	// 초기 상태는 꺼져있는 상태
	bCanChangeColor = false;
}

// Called when the game starts or when spawned
void APPTrafficLightController::BeginPlay()
{
	Super::BeginPlay();
	
	SetTrafficLightControllerState(ETrafficLightControllerState::TLC_OFF);	// 초기 상태를 꺼져있는 상태로 설정
}

void APPTrafficLightController::Charge()
{
	// 제어기의 전원이 켜져 있고 색상을 변경할 수 있는 상태(TLC_ON)라면 색상 변경 함수 호출
	if (TrafficLightControllerState == ETrafficLightControllerState::TLC_ON && bCanChangeColor)
	{
		ChangeTrafficLightColor();
		if (ColorChangeSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ColorChangeSound, GetActorLocation());	// 전원 켜질 때 사운드 재생
		}
		SetTrafficLightControllerState(ETrafficLightControllerState::TLC_WORKING);	// 상태를 작업 중으로 변경

		GetWorldTimerManager().ClearTimer(WorkingCooldownTimerHandle);	// 타이머를 초기화
		WorkingCooldownDelegate.BindUObject(this, &APPTrafficLightController::SetTrafficLightControllerState, ETrafficLightControllerState::TLC_ON);
		GetWorldTimerManager().SetTimer(WorkingCooldownTimerHandle, WorkingCooldownDelegate, 1.0f, false);
	}
}

void APPTrafficLightController::ConfigureConstraintSettings()
{
	PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 2.f);
	PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 2.f);
	PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 2.f);

	PhysicsConstraint->SetLinearDriveParams(1000.f, 5000.f, 0.f); // 얼마나 세게, 진동 감쇠, 최대 힘 제한
	PhysicsConstraint->SetLinearPositionDrive(true, true, true);
	PhysicsConstraint->SetLinearPositionTarget(FVector(0.f, 0.f, 0.f)); // 위치 드라이브의 목표 위치;

	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);

	PhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::SLERP);
	PhysicsConstraint->SetAngularOrientationDrive(true, true);
}

void APPTrafficLightController::SetTrafficLightControllerState(ETrafficLightControllerState NewState)
{
	TrafficLightControllerState = NewState;

	switch (TrafficLightControllerState)
	{
	case ETrafficLightControllerState::TLC_OFF:
	{
		if (TrafficLightControllerScreenStaticMesh && TrafficLightControllerOffMaterial)
		{
			TrafficLightControllerScreenStaticMesh->SetMaterial(0, TrafficLightControllerOffMaterial);
		}
		UE_LOG(LogTemp, Log, TEXT("Traffic Light Controller State: Off"));
	}
	break;
	case ETrafficLightControllerState::TLC_ON:
	{
		if (TrafficLightControllerScreenStaticMesh && TrafficLightControllerOnMaterial)
		{
			TrafficLightControllerScreenStaticMesh->SetMaterial(0, TrafficLightControllerOnMaterial);
		}
		UE_LOG(LogTemp, Log, TEXT("Traffic Light Controller State: On"));
	}
	break;
	case ETrafficLightControllerState::TLC_WORKING:
	{
		if (TrafficLightControllerScreenStaticMesh && TrafficLightControllerWorkingMaterial)
		{
			TrafficLightControllerScreenStaticMesh->SetMaterial(0, TrafficLightControllerWorkingMaterial);
		}
		UE_LOG(LogTemp, Log, TEXT("Traffic Light Controller State: Working"));
	}
	break;
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Traffic Light Controller State"));
	}
	break;
	}

}

void APPTrafficLightController::SetCanChangeColor(bool bCanChange)
{
	bCanChangeColor = bCanChange;
	if (bCanChangeColor)
	{
		UE_LOG(LogTemp, Log, TEXT("Traffic Light Controller can change color now."));
	}
	else
	{
		if (TrafficLightControllerState == ETrafficLightControllerState::TLC_WORKING)
		{
			GetWorldTimerManager().ClearTimer(WorkingCooldownTimerHandle);
			WorkingCooldownTimerHandle.Invalidate();	// 타이머 핸들 무효화
		}
		UE_LOG(LogTemp, Log, TEXT("Traffic Light Controller cannot change color now."));
	}
}

// Called every frame
void APPTrafficLightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPTrafficLightController::ChangeTrafficLightColor()
{
	// 신호등 색상 변경
	// 타입에 따라 변경 방식이 다름
	for (TObjectPtr<APPTrafficLightBase> TrafficLight : TrafficLights)
	{
		if (IsValid(TrafficLight))
		{
			UE_LOG(LogTemp, Log, TEXT("Cast Completed"));
			ETrafficLightType TrafficLightType = TrafficLight->GetTrafficLightType();
			ETrafficLightColor CurrentTrafficLightColor = TrafficLight->GetCurrentTrafficLightColor();
			switch (TrafficLightType)
			{
				//두 가지 색상의 신호등
			case ETrafficLightType::TL_TWO:
			{
				switch (CurrentTrafficLightColor)
				{
				case ETrafficLightColor::TC_GREEN:
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
					break;
				case ETrafficLightColor::TC_RED:
					CurrentTrafficLightColor = ETrafficLightColor::TC_GREEN;
					break;
				default:
					break;
				}
			}
				break;

				// 세가지 색상의 신호등(차단 봉 없음)
			case ETrafficLightType::TL_THREE:
			{
				switch (CurrentTrafficLightColor)
				{
				case ETrafficLightColor::TC_GREEN:
					CurrentTrafficLightColor = ETrafficLightColor::TC_YELLOW;
					break;
				case ETrafficLightColor::TC_YELLOW:
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
					break;
				case ETrafficLightColor::TC_RED:
					CurrentTrafficLightColor = ETrafficLightColor::TC_GREEN;
					break;
				default:
					break;
				}
			}
				break;

				// 세가지 색상의 신호등(차단 봉 있음)
			case ETrafficLightType::TL_THREE_WITH_BAR:
			{
				switch (CurrentTrafficLightColor)
				{
				case ETrafficLightColor::TC_GREEN:
					CurrentTrafficLightColor = ETrafficLightColor::TC_YELLOW;
					break;
				case ETrafficLightColor::TC_YELLOW:
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
					break;
				case ETrafficLightColor::TC_RED:
					CurrentTrafficLightColor = ETrafficLightColor::TC_GREEN;
					break;
				default:
					break;
				}
			}

			default:
				break;
			}
			// 최종적으로 해당하는 색깔로 변경
			TrafficLight->ChangeColor(CurrentTrafficLightColor);
		}
	}

	// 모두 바꾸고 난 후 신호등 색깔 체크 델리게이트
	OnTrafficLightColorChangedDelegate.ExecuteIfBound(ETrafficLightColor::TC_RED);
}

TArray<TObjectPtr<class APPTrafficLightBase>> APPTrafficLightController::GetTrafficLightsByController() const
{
	return TrafficLights;
}

void APPTrafficLightController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap Begin"));
	if (OtherActor && (OtherActor != this) && TrafficLightControllerState == ETrafficLightControllerState::TLC_OFF)
	{
		UE_LOG(LogTemp, Log, TEXT("OtherActor"));
		if (APPBattery* Battery = Cast<APPBattery>(OtherActor))
		{
			UE_LOG(LogTemp, Log, TEXT("Battery Overlap Begin"));

			FGameplayTag BatteryTag = FGameplayTag::RequestGameplayTag(FName("TrafficLight.Battery"));
			// 신호등 제어기 배터리 태그가 있는지 확인
			if (Battery->MyTags.HasTag(BatteryTag))
			{
				Battery->SetActorEnableCollision(false);	// 배터리의 충돌을 비활성화
				// 부드럽게 이동시키고 싶은데 어캐하지
				Battery->SetActorLocation(SnapPoint->GetComponentLocation());	// 그냥 붙는 효과

				UPrimitiveComponent* BatteryRoot = Cast<UPrimitiveComponent>(Battery->GetRootComponent());
				Battery->SetActorRotation(TrafficLightControllerStaticMesh->GetComponentRotation());
				BatteryRoot->SetSimulatePhysics(true);
				PhysicsConstraint->SetWorldLocation(ConstraintPoint->GetComponentLocation());
				PhysicsConstraint->SetConstrainedComponents(
					Cast<UPrimitiveComponent>(ConstraintPoint),
					NAME_None,
					BatteryRoot,
					NAME_None
				);
				ConfigureConstraintSettings(); // 여기서 물리 세팅
				BatteryRoot->SetEnableGravity(false);	// 중력 비활성화

				SetTrafficLightControllerState(ETrafficLightControllerState::TLC_ON);
				bCanChangeColor = true;	// 신호등 색상 변경 가능

				// 사운드 재생부
				if (PowerOnSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, PowerOnSound, GetActorLocation());	// 전원 켜질 때 사운드 재생
				}
				if (AudioComponent)
				{
					AudioComponent->SetSound(IdleSound);
					AudioComponent->Play();	// 사운드 재생
				}
				
				Battery->SetIsGrabbed(false);	// 배터리의 IsGrabbed를 false로 설정
				UE_LOG(LogTemp, Log, TEXT("Powered On"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Not Traffic Light Controller Battery"));
			}
		}
	}

}

void APPTrafficLightController::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

