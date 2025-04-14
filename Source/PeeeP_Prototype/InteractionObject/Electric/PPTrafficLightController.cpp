// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPTrafficLightController.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionObject/PPTrafficLight.h"
#include "InteractionObject/ETrafficLight.h"
#include "InteractionObject/PPTrafficLightManager.h"
#include "Components/BoxComponent.h"
#include "InteractionObject/PPBattery.h"
#include "GameplayTagContainer.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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

	BatteryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BatteryTrigger"));
	//BatteryTrigger->SetupAttachment(TrafficLightControllerStaticMesh);
	BatteryTrigger->OnComponentBeginOverlap.AddDynamic(this, &APPTrafficLightController::OnOverlapBegin);

	SnapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SnapPoint"));
	SnapPoint->SetupAttachment(RootComponent);

	ConstraintPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ConstraintPoint"));
	ConstraintPoint->SetupAttachment(RootComponent);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetConstrainedComponents(nullptr, NAME_None, nullptr, NAME_None);
	PhysicsConstraint->SetDisableCollision(true);


	bIsPowerOn = false;	// 초기값은 꺼져있음
}

// Called when the game starts or when spawned
void APPTrafficLightController::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPTrafficLightController::Charge()
{
	UE_LOG(LogTemp, Log, TEXT("TEST"));

	// 이후 신호 변화
	if (bIsPowerOn)
	{
		ChangeTrafficLightColor();
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

// Called every frame
void APPTrafficLightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPTrafficLightController::ChangeTrafficLightColor()
{
	// 신호등 색상 변경
	// 타입에 따라 변경 방식이 다름
	for (TObjectPtr<AActor> TrafficLightActor : TrafficLights)
	{
		APPTrafficLight* TrafficLight = Cast<APPTrafficLight>(TrafficLightActor);
		if (IsValid(TrafficLight))
		{
			UE_LOG(LogTemp, Log, TEXT("Cast Completed"));
			ETrafficLightType TrafficLightType = TrafficLight->GetTrafficLightType();
			ETrafficLightColor CurrentTrafficLightColor = TrafficLight->GetCurrentTrafficLightColor();
			switch (TrafficLightType)
			{
				//두 가지 색상의 신호등
			case ETrafficLightType::TL_TWO:
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
				break;
				// 세가지 색상의 신호등
			case ETrafficLightType::TL_THREE:
				if (CurrentTrafficLightColor == ETrafficLightColor::TC_GREEN)
				{
					CurrentTrafficLightColor = ETrafficLightColor::TC_RED;
				}
				else if (CurrentTrafficLightColor == ETrafficLightColor::TC_OFF)
				{
					// 신호등이 꺼져있을 때(현재 이 기능은 사용하지 않음)
				}
				else
				{
					uint8 TempColorNum = static_cast<uint8>(CurrentTrafficLightColor);
					TempColorNum--;
					CurrentTrafficLightColor = static_cast<ETrafficLightColor>(TempColorNum);
				}
				break;

			default:
				break;
			}
			// 최종적으로 해당하는 색깔로 변경
			TrafficLight->ChangeColor(CurrentTrafficLightColor);
		}
	}
}

TArray<TObjectPtr<class AActor>> APPTrafficLightController::GetTrafficLightsByController() const
{
	return TrafficLights;
}

void APPTrafficLightController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap Begin"));
	if (OtherActor && (OtherActor != this) && !bIsPowerOn)
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
				Battery->SetActorRotation(FRotator(0.f, 0.f, 0.f));
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

				bIsPowerOn = true;
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

