// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPLevelData.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPLevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPPLevelData();

	// 레벨 시작 시 플레이어가 보유할 체력을 강제 설정 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	uint8 bForcePlayerElectricCapacity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (EditCondition = "bForcePlayerElectricCapacity"))
	float StartElectricCapacity = 0.1f;


	// Tip UI 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tip Widget")
	uint8 bUseTipWidget : 1;
	// 레벨 시작 시 Tip UI 위젯
	UPROPERTY(EditAnywhere, Category = "Tip Widget", meta = (EditCondition = "bUseTipWidget"))
	TSubclassOf<class UUserWidget> LevelStartTipWidget;

	// Tip UI 생성 시간 오프셋
	UPROPERTY(EditAnywhere, Category = "Tip Widget", meta = (EditCondition = "bUseTipWidget"))
	float LevelStartTipWidgetAppearOffsetTime = 0.f;
	// Tip UI 표기 시간
	UPROPERTY(EditAnywhere, Category = "Tip Widget", meta = (EditCondition = "bUseTipWidget"))
	float LevelStartTipWidgetDisplayTime = 0.f;

};
