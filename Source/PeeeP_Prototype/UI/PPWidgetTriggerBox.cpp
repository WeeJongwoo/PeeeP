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

	bCanSpawnOnlyOnce = true; // 기본값으로 한 번만 생성 가능하도록 설정

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
		if (SpawnedWidget && SpawnedWidget->IsInViewport())
		{
			UE_LOG(LogTemp, Log, TEXT("[APPWidgetTriggerBox] Widget already exists for %s"), *OtherActor->GetName());
			return;
		}

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
				if (bCanSpawnOnlyOnce)
				{
					// TriggerBox를 제거하여 더 이상 오버랩 이벤트가 발생하지 않도록 함
					TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

