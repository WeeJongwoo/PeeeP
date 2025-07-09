// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPLoadingWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Components/Image.h"

UPPLoadingWidget::UPPLoadingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}


void UPPLoadingWidget::PlayFadeInAnimation()
{
	if (IsValid(FadeIn))
	{
		PlayAnimation(FadeIn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeIn animation is not valid!"));
	}

}

void UPPLoadingWidget::PlayFadeOutAnimation()
{
	if (IsValid(FadeOut))
	{
		PlayAnimation(FadeOut);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeOut animation is not valid!"));
	}
}

void UPPLoadingWidget::SetOnFadeInFinished(FSimpleDelegate InDelegate)
{
	OnFadeInFinished = InDelegate;
	if (FadeIn)
	{
		FWidgetAnimationDynamicEvent FadeInEvent;
		FadeInEvent.BindUFunction(this, FName("OnFadeInFinishedCallback"));

		BindToAnimationFinished(FadeIn, FadeInEvent);
	}
	
}

void UPPLoadingWidget::SetOnFadeOutFinished(FSimpleDelegate InDelegate)
{
	OnFadeOutFinished = InDelegate;
	if (FadeOut)
	{
		FWidgetAnimationDynamicEvent FadeOutEvent;
		FadeOutEvent.BindUFunction(this, FName("OnFadeOutFinishedCallback"));

		BindToAnimationFinished(FadeOut, FadeOutEvent);
	}
}

void UPPLoadingWidget::OnFadeInFinishedCallback()
{
	if (OnFadeInFinished.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("FadeIn Opacity %f"), FadeImage->ColorAndOpacity.A);
		OnFadeInFinished.Execute();
	}
}

void UPPLoadingWidget::OnFadeOutFinishedCallback()
{
	if (OnFadeOutFinished.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("FadeOut Opacity %f, Time %f"), FadeImage->ColorAndOpacity.A, GetAnimationCurrentTime(FadeOut));
		OnFadeOutFinished.Execute();
	}
}
