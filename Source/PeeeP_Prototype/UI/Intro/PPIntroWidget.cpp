// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Intro/PPIntroWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Kismet/GameplayStatics.h"
#include "MediaSoundComponent.h"


void UPPIntroWidget::NativeConstruct()
{
	bIsFocusable = true;
	SetFocus();
}

FReply UPPIntroWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	StartGame();

	return FReply::Handled();
}

FReply UPPIntroWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	StartGame();

	return FReply::Handled();
}

void UPPIntroWidget::StartGame()
{
	VideoMedia->Close();
	if(LevelToOpen.IsPending())
	{
		LevelToOpen.LoadSynchronous();
	}

	if (LevelToOpen != nullptr && LevelToOpen.IsValid())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToOpen.Get());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UPPIntroWidget] LevelToOpen is not valid"));
	}
}

void UPPIntroWidget::PlayVideo()
{

	if (VideoMedia)
	{
		if (VideoMedia->OpenSource(VideoMediaSource))
		{
			VideoMediaTexture->SetMediaPlayer(VideoMedia);
			VideoMediaTexture->UpdateResource();

			VideoMedia->SetNativeVolume(1.0f);
			VideoMedia->SetLooping(false);
			VideoMedia->Play();
			
			UE_LOG(LogTemp, Log, TEXT("Play Video"));

			VideoMedia->OnEndReached.AddDynamic(this, &UPPIntroWidget::PlayLoop);
		}
	}
}

void UPPIntroWidget::PlayLoop()
{
	if (VideoMedia)
	{
		if (VideoMedia->OpenSource(VideoMediaLoopSource))
		{
			VideoMedia->Seek(FTimespan::FromMilliseconds(0));

			VideoMediaTexture->SetMediaPlayer(VideoMedia);
			//VideoMediaTexture->UpdateResource();

			VideoMedia->SetLooping(true);
			VideoMedia->Play();
			UE_LOG(LogTemp, Log, TEXT("Play Video"));

		}
	}
}
