#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "PPSnowMobile.generated.h"

class APPCharacterPlayer;

UCLASS()
class PEEEP_PROTOTYPE_API APPSnowMobile : public AActor
{
	GENERATED_BODY()

public:
	APPSnowMobile();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snowmobile", meta = (AllowPrivateAccess = "true"))
	USplineComponent* PathSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snowmobile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SnowmobileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Movement", meta = (ClampMin = "0", AllowPrivateAccess = "true"))
	float MovementSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Movement", meta = (AllowPrivateAccess = "true"))
	bool bLoopPath = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Movement", meta = (AllowPrivateAccess = "true"))
	FRotator MeshRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Damage", meta = (ClampMin = "0", AllowPrivateAccess = "true"))
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Collision", meta = (ClampMin = "0", AllowPrivateAccess = "true"))
	float KnockbackStrength = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowmobile|Collision", meta = (ClampMin = "0", AllowPrivateAccess = "true"))
	float StopDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snowmobile|Sound", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> SnowmobileIdleSound;

	float DistanceAlongSpline = 0.f;
	bool bIsPaused = false;
	FTimerHandle PauseTimerHandle;
	FVector LastDirection = FVector::ForwardVector;

	void ResumeMovement();

	UFUNCTION()
	void OnSnowmobileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
