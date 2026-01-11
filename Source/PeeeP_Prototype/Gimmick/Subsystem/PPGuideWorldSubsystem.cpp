// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Subsystem/PPGuideWorldSubsystem.h"
#include "Component/PPGuideObjectComponent.h"
#include "TimerManager.h"

void UPPGuideWorldSubsystem::RegisterGuideObject(UPPGuideObjectComponent* GuideObject)
{
	GuideObjectSet.Add(GuideObject);
}

void UPPGuideWorldSubsystem::UnregisterGuideObject(UPPGuideObjectComponent* GuideObject)
{
	GuideObjectSet.Remove(GuideObject);
}

void UPPGuideWorldSubsystem::GuideActive()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(GuideTimerHandle))
	{
		return;
	}

	for (TWeakObjectPtr<UPPGuideObjectComponent> WeakGuideObjectPtr : GuideObjectSet)
	{
		if (UPPGuideObjectComponent* GuideObjectPtr = WeakGuideObjectPtr.Get())
		{
			GuideObjectPtr->GuideSetHidden(false);

		}
	}
	OnGuideActiveDelegate.Broadcast();

	TWeakObjectPtr<UPPGuideWorldSubsystem> ThisWeakPtr(this);

	FTimerDelegate GuideTimerDelegate;
	GuideTimerDelegate.BindLambda([ThisWeakPtr]() {

		if (UPPGuideWorldSubsystem* ThisPtr = ThisWeakPtr.Get())
		{
			for (TWeakObjectPtr<UPPGuideObjectComponent> WeakGuideObjectPtr : ThisPtr->GuideObjectSet)
			{
				if (UPPGuideObjectComponent* GuideObjectPtr = WeakGuideObjectPtr.Get())
				{
					GuideObjectPtr->GuideSetHidden(true);
				}
			}

			ThisPtr->OnGuideHiddenDelegate.Broadcast();
			ThisPtr->GetWorld()->GetTimerManager().ClearTimer(ThisPtr->GuideTimerHandle);
		}
	});

	GetWorld()->GetTimerManager().SetTimer(GuideTimerHandle, GuideTimerDelegate, 3.0f, false);
}
