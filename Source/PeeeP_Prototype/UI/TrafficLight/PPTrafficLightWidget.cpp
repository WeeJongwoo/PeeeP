// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TrafficLight/PPTrafficLightWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "InteractionObject/PPTrafficLightManager.h"
#include "InteractionObject/PPTrafficLightBase.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionObject/ETrafficLight.h"
#include "UI/TrafficLight/PPTrafficLightThree.h"
#include "UI/TrafficLight/PPTrafficLightTwo.h"
#include "Animation/WidgetAnimation.h"

void UPPTrafficLightWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TrafficLightManager = Cast<APPTrafficLightManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APPTrafficLightManager::StaticClass()));
	if (TrafficLightManager)
	{
		// Bind the delegate to refresh traffic light colors
		TrafficLightManager->RefreshTrafficLightsColorDelegate.AddUObject(this, &UPPTrafficLightWidget::RefreshTrafficLightColor);
		TrafficLightManager->DisableTrafficLightWidgetDelegate.AddUObject(this, &UPPTrafficLightWidget::DisableTrafficLightWidget);
		UE_LOG(LogTemp, Log, TEXT("[UPPTrafficLightWidget] TrafficLightManager found: %s"), *TrafficLightManager->GetName());
	}

	if (IsValid(TrafficLightThreeWidgetClass) && IsValid(TrafficLightTwoWidgetClass))
	{
		UE_LOG(LogTemp, Log, TEXT("[UPPTrafficLightWidget] Widget classes are valid."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPPTrafficLightWidget] Widget classes are not valid!"));
		return;
	}

	TArray<TObjectPtr<class APPTrafficLightBase>> TrafficLights = TrafficLightManager->GetTrafficLights();
	// TrafficLightsWithWidget에 신호등과 위젯을 매핑
	for (APPTrafficLightBase* TrafficLight : TrafficLights)
	{
		if (IsValid(TrafficLight))
		{
			ETrafficLightType TrafficLightType = TrafficLight->GetTrafficLightType();
			switch (TrafficLightType)
			{
			case ETrafficLightType::TL_THREE_WITH_BAR:
			case ETrafficLightType::TL_THREE:
			{
				UPPTrafficLightThree* TrafficLightWidget = CreateWidget<UPPTrafficLightThree>(GetWorld(), TrafficLightThreeWidgetClass);
				if (TrafficLightWidget)
				{
					// Map the TrafficLight to the TrafficLightWidget
					TrafficLightsWithWidget.Add(TrafficLight, TrafficLightWidget);
					// Add the widget to the TrafficLightBox
					if (TrafficLightsBox)
					{
						TrafficLightsBox->AddChild(TrafficLightWidget);
						UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(TrafficLightWidget->Slot);
						if (HorizontalBoxSlot)
						{
							HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						}
						TrafficLightWidget->AddToViewport();
					}
				}
			}
				break;
			case ETrafficLightType::TL_TWO:
			{
				UPPTrafficLightTwo* TrafficLightWidget = CreateWidget<UPPTrafficLightTwo>(GetWorld(), TrafficLightTwoWidgetClass);
				if (TrafficLightWidget)
				{
					// Map the TrafficLight to the TrafficLightWidget
					TrafficLightsWithWidget.Add(TrafficLight, TrafficLightWidget);
					// Add the widget to the TrafficLightBox
					if (TrafficLightsBox)
					{
						TrafficLightsBox->AddChild(TrafficLightWidget);
						UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(TrafficLightWidget->Slot);
						if (HorizontalBoxSlot)
						{
							HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						}
						TrafficLightWidget->AddToViewport();
					}
				}
			}
				break;
			default:
				break;
			}
		}
	}

	// AddToViewPort시 애니메이션 재생
	if (BackgroundAppearAnim)
	{
		PlayAnimation(BackgroundAppearAnim);
	}
	// 초기 신호등 색상 갱신
	RefreshTrafficLightColor();
}

void UPPTrafficLightWidget::OnExitAnimationFinished()
{
	ExitAnimationTimerHandle.Invalidate();
	RemoveFromParent();
}

void UPPTrafficLightWidget::RefreshTrafficLightColor()
{
	// TrafficLightsWithWidget에 있는 신호등 위젯을 모두 갱신
	for (const auto& Pair : TrafficLightsWithWidget)
	{
		APPTrafficLightBase* TrafficLight = Pair.Key;
		UPPCustomWidget* TrafficLightWidget = Pair.Value;
		if (IsValid(TrafficLight) && IsValid(TrafficLightWidget))
		{
			ETrafficLightColor CurrentColor = TrafficLight->GetCurrentTrafficLightColor();
			if (UPPTrafficLightThree* ThreeWidget = Cast<UPPTrafficLightThree>(TrafficLightWidget))
			{
				ThreeWidget->SetTrafficLightColorVisible(CurrentColor);
			}
			else if (UPPTrafficLightTwo* TwoWidget = Cast<UPPTrafficLightTwo>(TrafficLightWidget))
			{
				TwoWidget->SetTrafficLightColorVisible(CurrentColor);
			}
		}
	}
}

void UPPTrafficLightWidget::DisableTrafficLightWidget()
{
	if (BackgroundDisappearAnim)
	{
		PlayAnimation(BackgroundDisappearAnim);
		GetWorld()->GetTimerManager().ClearTimer(ExitAnimationTimerHandle); // 이전 타이머를 정리
		GetWorld()->GetTimerManager().SetTimer(ExitAnimationTimerHandle, this, &UPPTrafficLightWidget::OnExitAnimationFinished, BackgroundDisappearAnim->GetEndTime(), false);
	}
}

