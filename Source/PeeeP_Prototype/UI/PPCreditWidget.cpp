// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPCreditWidget.h"
#include "Kismet/GameplayStatics.h"

UPPCreditWidget::UPPCreditWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 크레딧 음악 로드
	static ConstructorHelpers::FObjectFinder<USoundBase> CreditMusicAsset(TEXT("/Game/SFX/BGM/cues/SC_Credit.SC_Credit"));
	if (CreditMusicAsset.Succeeded())
	{
		CreditMusic = CreditMusicAsset.Object;
	}
}

void UPPCreditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreditAnim)
	{
		PlayAnimation(CreditAnim);
	}
	if (CreditMusic)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), CreditMusic);
	}	
}
