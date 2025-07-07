// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "InteractionObject/ETrafficLight.h"
#include "PPTrafficLightController.generated.h"

DECLARE_DELEGATE_OneParam(FOnTrafficLightColorChangedDelegate, ETrafficLightColor);

UENUM(BlueprintType)
enum class ETrafficLightControllerState : uint8
{
	TLC_OFF UMETA(DisplayName = "Off"),	// 전원이 꺼져 있는 상태
	TLC_ON UMETA(DisplayName = "On"),	// 전원이 켜져 있는 상태
	TLC_WORKING UMETA(DisplayName = "Working")	// 전기 차징으로 신호를 바꾸는 중인 상태
};

UCLASS()
class PEEEP_PROTOTYPE_API APPTrafficLightController : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrafficLightController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> TrafficLightControllerStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> TrafficLightControllerStateFrameStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> TrafficLightControllerScreenStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UMaterialInstance> TrafficLightControllerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UMaterialInterface> TrafficLightControllerOffMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UMaterialInterface> TrafficLightControllerOnMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UMaterialInterface> TrafficLightControllerWorkingMaterial;

	// Audio Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> IdleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> PowerOnSound;	// 전원이 켜졌을 때 재생되는 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> ColorChangeSound;	// 신호등 변경 시 재생되는 사운드

	// 배터리가 닿는 영역
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UBoxComponent> BatteryTrigger;	

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> SnapPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> ConstraintPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPhysicsConstraintComponent> PhysicsConstraint;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 제어기가 관리하는 신호등 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrafficLightController")
	TArray<TObjectPtr<class APPTrafficLightBase>> TrafficLights;

	// 신호가 바뀌었을 때 신호등들의 색깔에 따른 판정을 하기 위한 TrafficLightManager
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TrafficLightController")
	TObjectPtr<class APPTrafficLightManager> TrafficLightManager;

	// TrafficLightController의 현재 상태를 나타내는 변수
	// 상단 ETrafficLightControllerState enum을 참고
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TrafficLightController")
	ETrafficLightControllerState TrafficLightControllerState;

	UPROPERTY()
	uint8 bCanChangeColor : 1;	// 신호등 색상을 변경할 수 있는지 여부를 나타내는 플래그(퍼즐 클리어 시 전원은 켜진 채 색상 변경 가능 유무를 정하기 위해)

	void Charge() override;

	void ConfigureConstraintSettings();

	FTimerHandle WorkingCooldownTimerHandle;	// 색상 변경 시 쿨타임을 위한 타이머 핸들
	FTimerDelegate WorkingCooldownDelegate;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeTrafficLightColor();

	// TrafficLightsByController를 가져오는 getter함수
	TArray<TObjectPtr<class APPTrafficLightBase>> GetTrafficLightsByController() const;

	FOnTrafficLightColorChangedDelegate OnTrafficLightColorChangedDelegate;

	void SetTrafficLightControllerState(ETrafficLightControllerState NewState);

	// 신호등 색상을 변경할 수 있는지 여부를 나타내는 플래그 설정 함수(퍼즐 클리어 시 전원은 켜진 채 색상 변경 가능 유무를 정하기 위해)
	void SetCanChangeColor(bool bCanChange);
};
