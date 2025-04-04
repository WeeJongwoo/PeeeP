// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsData/PPParkourPartsData.h"
#include "Parts/PartsComponent/PPParkourParts.h"
#include "InputMappingContext.h"
#include "Animation/PPParkourAnimInstance.h"

UPPParkourPartsData::UPPParkourPartsData()
{
	PartsComponentClass = UPPParkourParts::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> PartsMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Parts/Parkour/IMC_ParkourInputMapping.IMC_ParkourInputMapping'"));
	if (PartsMappingContextRef.Object)
	{
		PartsMappingContext = PartsMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChargingJumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Parts/Parkour/IA_ParkourJumpAction.IA_ParkourJumpAction'"));
	if (ChargingJumpActionRef.Object)
	{
		ChargingJumpAction = ChargingJumpActionRef.Object;
	}
}
