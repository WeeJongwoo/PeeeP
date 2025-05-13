// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/PPKeyManualUI.h"
#include "PPAlertUIArea.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPAlertUIArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPAlertUIArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle AutoHiddenTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr <class UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KeyManuals")
	float KeyManualShowTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KeyManuals")
	EKeyManual KeyManual;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetKeyMaunalWidgetHiddenByTimer(class IPPElectricHUDInterface* ElectircHUDInterface);
};
