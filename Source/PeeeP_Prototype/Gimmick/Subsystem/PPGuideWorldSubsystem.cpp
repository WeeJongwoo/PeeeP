// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/Subsystem/PPGuideWorldSubsystem.h"
#include "Component/PPGuideObjectComponent.h"

void UPPGuideWorldSubsystem::RegisterGuideObject(UPPGuideObjectComponent* GuideObject)
{
	GuideObjectSet.Add(GuideObject);
}

void UPPGuideWorldSubsystem::UnregisterGuideObject(UPPGuideObjectComponent* GuideObject)
{
	GuideObjectSet.Remove(GuideObject);
}

void UPPGuideWorldSubsystem::SetGuideActive(bool Active)
{
	for (TWeakObjectPtr<UPPGuideObjectComponent> WeakGuideObjectPtr : GuideObjectSet)
	{
		if (UPPGuideObjectComponent* GuideObjectPtr = WeakGuideObjectPtr.Get())
		{
			GuideObjectPtr->GuideSetHidden(!Active);
		}
	}
}
