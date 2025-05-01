#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPTurbine.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPTurbine : public AActor
{
	GENERATED_BODY()

public:
	APPTurbine();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbine")
	UStaticMeshComponent* TurbineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FVector RotationAxis = FVector(1.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (ClampMin = "0.0"))
	float RotationSpeed = 90.f;
};