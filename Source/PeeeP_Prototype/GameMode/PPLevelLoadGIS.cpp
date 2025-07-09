// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPLevelLoadGIS.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "UI/PPLoadingWidget.h"


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

	/*FString LoadingWidgetPath = TEXT("/Game/UI/Loading/WBP_LoadingWidget.WBP_LoadingWidget_C");

	UClass* FoundLoadingWidgetClass = StaticLoadClass(UPPLoadingWidget::StaticClass(), nullptr, *LoadingWidgetPath);
	if (FoundLoadingWidgetClass)
	{
		LoadingWidgetClass = FoundLoadingWidgetClass;
		if (!LoadingWidgetClass)
		{
			UE_LOG(LogTemp, Log, TEXT("LoadingWidgetClass loaded unsuccessfully"));
		}
	}*/

}


void UPPLevelLoadGIS::LoadLevel(const TSoftObjectPtr<class UWorld>& InTartgetLevel)
{

	UGameInstance* GameInstance = GetGameInstance();

	LoadingWidget = CreateWidget<UPPLoadingWidget>(GameInstance, LoadingWidgetClass);

	if (IsValid(LoadingWidget))
	{
		LoadingWidget->AddToViewport(100);
		LoadingWidget->PlayFadeOutAnimation();
		LoadingWidget->SetOnFadeOutFinished(FSimpleDelegate::CreateUObject(this, &UPPLevelLoadGIS::OnMinTimeReached));
	}

	//GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, this, &UPPLevelLoadGIS::OnMinTimeReached, 3.0f, false);

	LevelHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		InTartgetLevel.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UPPLevelLoadGIS::OnLevelLoaded));

	//LevelHandle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateUObject(this, &UPPLevelLoadGIS::OnLoadingUpdate));

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPPLevelLoadGIS::OnPostLoadLevel);

	TargetLevel = InTartgetLevel;

}

void UPPLevelLoadGIS::OnLoadingUpdate(TSharedRef<FStreamableHandle> Handle)
{
	UE_LOG(LogTemp, Log, TEXT("Loading Update"));
}

void UPPLevelLoadGIS::OnLevelLoaded()
{
	bIsLoading = true;
	TryToLoadLevel();
}

void UPPLevelLoadGIS::OnPostLoadLevel(UWorld* LoadedWorld)
{
	//DeleteLoadingWidget();

	UGameInstance* GameInstance = GetGameInstance();
	LoadingWidget = CreateWidget<UPPLoadingWidget>(GameInstance, LoadingWidgetClass);

	if (IsValid(LoadingWidget))
	{
		LoadingWidget->AddToViewport(100);

		LoadingWidget->PlayFadeInAnimation();

		LoadingWidget->SetOnFadeInFinished(FSimpleDelegate::CreateUObject(this, &UPPLevelLoadGIS::DeleteLoadingWidget));
	}
}

void UPPLevelLoadGIS::DoOpenLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, TargetLevel, true);

	LevelHandle->ReleaseHandle();
	bMinTimeMet = false;
	bIsLoading = false;
}

void UPPLevelLoadGIS::OnMinTimeReached()
{
	bMinTimeMet = true;
	TryToLoadLevel();
}

void UPPLevelLoadGIS::TryToLoadLevel()
{
	if (bMinTimeMet && bIsLoading)
	{
		UE_LOG(LogTemp, Log, TEXT("TryToLoadLevel"));
		DoOpenLevel();
		/*if (IsValid(LoadingWidget))
		{
			LoadingWidget->AddToViewport(100);
			LoadingWidget->PlayFadeInAnimation();
		}*/

		//if (IsValid(LoadingWidget))
		//{
		//	LoadingWidget->PlayFadeInAnimation();
		//	//LoadingWidget->SetOnFadeInFinished(FSimpleDelegate::CreateUObject(this, &UPPLevelLoadGIS::DoOpenLevel));
		//}
	}
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
