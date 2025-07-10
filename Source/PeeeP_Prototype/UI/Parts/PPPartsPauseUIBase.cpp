// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Parts/PPPartsPauseUIBase.h"
#include "Components/Image.h"

UPPPartsPauseUIBase::UPPPartsPauseUIBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UPPPartsPauseUIBase::End()
{
	if (EndUIDelegate.IsBound())
	{
		EndUIDelegate.Execute();
	}
}

FReply UPPPartsPauseUIBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::SpaceBar)
	{
		CurrentPartsIndex++;
		SetPartsInfoImage(CurrentPartsIndex);
	}

	return FReply::Handled();
}

FReply UPPPartsPauseUIBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CurrentPartsIndex++;
		SetPartsInfoImage(CurrentPartsIndex);
	}

	return FReply::Handled();
}

void UPPPartsPauseUIBase::SetPartsInfoImage(const int16 InCurrentPartsIndex)
{
	if (PartsInfoImages.IsEmpty())
	{
		return;
	}

	if (InCurrentPartsIndex >= PartsInfoImages.Num())
	{
		End();
		return;
	}
	
	Parts_Info->SetBrushFromTexture(PartsInfoImages[InCurrentPartsIndex].Get(), true);
}

void UPPPartsPauseUIBase::SetPartsInfoImageArray(const TArray<TSoftObjectPtr<UTexture2D>>& InPartsInfo)
{
	if (InPartsInfo.IsEmpty())
	{
		return;
	}

	PartsInfoImages.Empty();

	for(auto& PartsInfo : InPartsInfo)
	{
		UTexture2D* Texture = PartsInfo.LoadSynchronous();
		if (Texture)
		{
			PartsInfoImages.Add(Texture);
		}
	}

	CurrentPartsIndex = 0;
	SetPartsInfoImage(CurrentPartsIndex);
}
