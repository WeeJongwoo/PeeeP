// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIStringRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUIStringRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Context;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
};
