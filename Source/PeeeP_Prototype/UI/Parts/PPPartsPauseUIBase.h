// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPPartsPauseUIBase.generated.h"


DECLARE_DELEGATE(FPartsUIEndDelegate);

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPPartsPauseUIBase : public UUserWidget
{
	GENERATED_BODY()
	
	void End();
	
public:

	FPartsUIEndDelegate EndUIDelegate;

};
