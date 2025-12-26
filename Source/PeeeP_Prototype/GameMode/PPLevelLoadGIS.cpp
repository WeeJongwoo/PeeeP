// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPLevelLoadGIS.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "UI/PPLoadingWidget.h"
#include "UObject/UObjectGlobals.h"


UPPLevelLoadGIS::UPPLevelLoadGIS()
{
	static ConstructorHelpers::FClassFinder<UPPLoadingWidget> LoadingWidgetClassRef(TEXT("/Game/UI/Loading/WBP_LoadingWidget.WBP_LoadingWidget_C"));
	if (LoadingWidgetClassRef.Class)
	{
		LoadingWidgetClass = LoadingWidgetClassRef.Class;
	}
}
void UPPLevelLoadGIS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPPLevelLoadGIS::OnPostLoadLevel);

	UE_LOG(LogTemp, Log, TEXT("PPLevelLoadGI Initialized"));
}

void UPPLevelLoadGIS::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	DeleteLoadingWidget();

	Super::Deinitialize();
}


void UPPLevelLoadGIS::LoadLevel(const TSoftObjectPtr<class UWorld>& InTartgetLevel)
{
	TargetLevel = InTartgetLevel;

	UGameInstance* GameInstance = GetGameInstance();

	if (InTartgetLevel.IsNull())
	{
		return;
	}

	DeleteLoadingWidget();

	LoadingWidget = CreateWidget<UPPLoadingWidget>(GameInstance, LoadingWidgetClass);

	if (IsValid(LoadingWidget))
	{
		LoadingWidget->AddToViewport(1);
		LoadingWidget->PlayFadeOutAnimation();
		LoadingWidget->SetOnFadeOutFinished(FSimpleDelegate::CreateUObject(this, &UPPLevelLoadGIS::TryToLoadLevel));
		bIsLoadingComplete = false;
	}

	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPPLevelLoadGIS::OnPostLoadLevel);
}

void UPPLevelLoadGIS::OnLoadingUpdate(TSharedRef<FStreamableHandle> Handle)
{
	UE_LOG(LogTemp, Log, TEXT("Loading Update"));
}

//void UPPLevelLoadGIS::OnLevelLoaded()
//{
//	bIsLoading = true;
//	TryToLoadLevel();
//}

void UPPLevelLoadGIS::OnPostLoadLevel(UWorld* LoadedWorld)
{
	if (TargetLevel.IsNull())
	{
		return;
	}

	DeleteLoadingWidget();

	UGameInstance* GameInstance = GetGameInstance();
	LoadingWidget = CreateWidget<UPPLoadingWidget>(GameInstance, LoadingWidgetClass);

	if (IsValid(LoadingWidget))
	{
		LoadingWidget->AddToViewport(100);

		TWeakObjectPtr<UPPLevelLoadGIS> WeakThis(this);

		FTimerDelegate PollLoadingDelegate;
		PollLoadingDelegate.BindLambda([WeakThis]()
			{
				if (!WeakThis.IsValid())
				{
					return;
				}

				UPPLevelLoadGIS* This = WeakThis.Get();

				if (This->bIsLoadingComplete)
				{
					return;
				}

				if (!IsAsyncLoading())
				{
					This->bIsLoadingComplete = true;
					if (UWorld* World = This->GetWorld())
					{
						World->GetTimerManager().ClearTimer(This->LoadingTimerHandle);
					}
					
					if (IsValid(This->LoadingWidget) && This->LoadingWidget->IsInViewport())
					{
						This->LoadingWidget->PlayFadeInAnimation();
						This->LoadingWidget->SetOnFadeInFinished(FSimpleDelegate::CreateUObject(This, &UPPLevelLoadGIS::DeleteLoadingWidget));
					}
				}
			}
		);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(LoadingTimerHandle, PollLoadingDelegate, 0.1f, true);
		}
	}
}

//void UPPLevelLoadGIS::DoOpenLevel()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Tartget Level Open"));
//	UGameplayStatics::OpenLevelBySoftObjectPtr(this, TargetLevel, true);
//
//	LevelHandle->ReleaseHandle();
//	bMinTimeMet = false;
//	bIsLoading = false;
//}
//
//void UPPLevelLoadGIS::OnMinTimeReached()
//{
//	bMinTimeMet = true;
//	TryToLoadLevel();
//}

void UPPLevelLoadGIS::TryToLoadLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, TargetLevel, true);
}

void UPPLevelLoadGIS::DeleteLoadingWidget()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LoadingTimerHandle);
	}

	if (IsValid(LoadingWidget))
	{
		if (LoadingWidget->IsInViewport())
		{
			LoadingWidget->RemoveFromViewport();
		}
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
		bIsLoadingComplete = false;
		UE_LOG(LogTemp, Log, TEXT("LoadingWidget deleted successfully"));
	}
}
