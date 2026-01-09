// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPTipWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPGameInstance.h"
#include "Components/TextBlock.h"
#include "UI/UIStringRow.h"



void UPPTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindToAnimationFinished(TipDisappearAnim, OnTipDisappearAnimFinished);
}

void UPPTipWidget::SetTipTitleText(const FName& TitleKey)
{
	UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance && TipTitleText)
	{
		if (FUIStringRow* UIStringRow = GameInstance->GetUIStringTable()->FindRow<FUIStringRow>(TitleKey, TEXT("")))
		{
			TipTitleText->SetText(FText::FromString(UIStringRow->Text.ToString()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UPPTimeWidget] UI String Key not found: %s"), *TitleKey.ToString());
		}
	}
}

void UPPTipWidget::SetTipContentsText(const FName& ContentsKey)
{
	UPPGameInstance* GameInstance = Cast<UPPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance && TipContentsText)
	{
		if (FUIStringRow* UIStringRow = GameInstance->GetUIStringTable()->FindRow<FUIStringRow>(ContentsKey, TEXT("")))
		{
			TipContentsText->SetText(FText::FromString(UIStringRow->Text.ToString()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UPPTimeWidget] UI String Key not found: %s"), *ContentsKey.ToString());
		}
	}
}

void UPPTipWidget::PlayWidgetAnimationIn()
{
	if (TipAppearAnim)
	{
		PlayAnimation(TipAppearAnim);
	}
}

void UPPTipWidget::PlayWidgetAnimation()
{
}

void UPPTipWidget::PlayWidgetAnimationOut()
{
	if (TipDisappearAnim)
	{
		PlayAnimation(TipDisappearAnim);
	}
}
