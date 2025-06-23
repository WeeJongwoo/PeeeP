// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPWidgetTriggerBox.h"
#include "Components/BoxComponent.h"
#include "UI/Inventory/PPCustomWidget.h"
#include "Character/PPCharacterPlayer.h"
#include "UI/TrafficLight/PPTrafficLightWidget.h"
#include "GameMode/PPPlayerController.h"

// Sets default values
APPWidgetTriggerBox::APPWidgetTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPWidgetTriggerBox::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APPWidgetTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPWidgetTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("[APPWidgetTriggerBox] Overlap Begin with %s"), *OtherActor->GetName());

	if (OtherActor && (OtherActor != this))
	{
		APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(OtherActor);
		if (PlayerCharacter)
		{
			if (WidgetClass)
			{
				APPPlayerController* PlayerController = PlayerCharacter->GetController<APPPlayerController>();
				if (!PlayerController)
				{
					UE_LOG(LogTemp, Warning, TEXT("[APPWidgetTriggerBox] PlayerController is null for %s"), *OtherActor->GetName());
					return;
				}
				SpawnedWidget = CreateWidget<UPPCustomWidget>(PlayerController, WidgetClass);
				if (SpawnedWidget)
				{
					SpawnedWidget->SetOwningActor(OtherActor);
					SpawnedWidget->AddToViewport();
					SpawnedWidget->SetVisibility(ESlateVisibility::Visible);
					UE_LOG(LogTemp, Log, TEXT("[APPWidgetTriggerBox] Widget added to viewport for %s"), *OtherActor->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[APPWidgetTriggerBox] Failed to cast Widget to UPPCustomWidget"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[APPWidgetTriggerBox] Overlap with invalid actor or component"));
	}

}

void APPWidgetTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void APPWidgetTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

