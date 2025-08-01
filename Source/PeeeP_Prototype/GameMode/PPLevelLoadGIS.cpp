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

	UE_LOG(LogTemp, Log, TEXT("PPLevelLoadGI Initialized"));
}


void UPPLevelLoadGIS::LoadLevel(const TSoftObjectPtr<class UWorld>& InTartgetLevel)
{

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
	}

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPPLevelLoadGIS::OnPostLoadLevel);

	TargetLevel = InTartgetLevel;

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
	DeleteLoadingWidget();

	UGameInstance* GameInstance = GetGameInstance();
	LoadingWidget = CreateWidget<UPPLoadingWidget>(GameInstance, LoadingWidgetClass);

	if (IsValid(LoadingWidget))
	{
		LoadingWidget->AddToViewport(100);

		FTimerDelegate PollLoadingDelegate;
		PollLoadingDelegate.BindLambda([this]()
			{
				if (!IsAsyncLoading())
				{
					GetWorld()->GetTimerManager().ClearTimer(LoadingTimerHandle);

					LoadingWidget->PlayFadeInAnimation();
					LoadingWidget->SetOnFadeInFinished(
						FSimpleDelegate::CreateUObject(this, &UPPLevelLoadGIS::DeleteLoadingWidget)
					);
				}
			});

		GetWorld()->GetTimerManager().SetTimer(
			LoadingTimerHandle,
			PollLoadingDelegate,
			0.1f,
			true
		);

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
	if (IsValid(LoadingWidget))
	{
		LoadingWidget->RemoveFromViewport();
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("LoadingWidget deleted successfully"));
	}
}
