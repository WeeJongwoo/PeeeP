// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPGuideObjectComponent.h"
#include "Gimmick/Subsystem/PPGuideWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPPGuideObjectComponent::UPPGuideObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPPGuideObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	GuideSetHidden(true);
}

void UPPGuideObjectComponent::OnRegister()
{
	Super::OnRegister();
	
	UWorld* World = GetWorld();
	UPPGuideWorldSubsystem* GuideSubsystem = World->GetSubsystem<UPPGuideWorldSubsystem>();

	if (GuideSubsystem)
	{
		GuideSubsystem->RegisterGuideObject(this);
	}
}

void UPPGuideObjectComponent::OnUnregister()
{
	Super::OnUnregister();

	UWorld* World = GetWorld();
	UPPGuideWorldSubsystem* GuideSubsystem = World->GetSubsystem<UPPGuideWorldSubsystem>();

	if (GuideSubsystem)
	{
		GuideSubsystem->UnregisterGuideObject(this);
	}
}


void UPPGuideObjectComponent::GuideSetHidden(bool bHidden)
{
	GetOwner()->SetActorHiddenInGame(bHidden);
	if (!bHidden)
	{
		if (GuideAppearSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GuideAppearSound, GetOwner()->GetActorLocation());
		}
	}
}

