// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsItemBox/PPPartsItemBox.h"
#include "Components/BoxComponent.h"
#include "Character/PPCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPPlayerController.h"

// Sets default values
APPPartsItemBox::APPPartsItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemBoxMesh"));
	SetRootComponent(ItemBoxMesh);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(ItemBoxMesh);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APPPartsItemBox::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APPPartsItemBox::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PartsData))
	{
		ItemBoxMesh->SetStaticMesh(PartsData->PartsItemMesh);
	}
}

// Called every frame
void APPPartsItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the item box
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += DeltaTime * 50.0f;
	SetActorRotation(NewRotation);

}

void APPPartsItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (APPCharacterPlayer* player = Cast<APPCharacterPlayer>(OtherActor))
		{

			UE_LOG(LogTemp, Log, TEXT("Player Overlap Begin"));
			InventoryComponent = player->GetInventoryComponent();
			int32 TempOutItemQuntity = 0;

			// Check If Player already has the parts
			// If Player already has the parts, do not add it again
			InventoryComponent->TryAddItem(PartsData.GetFName(), 1, TempOutItemQuntity);

			//InventoryComponent->AddItem(PartsData.GetFName(), 1, TempOutItemQuntity);
			InventoryComponent->SaveInventoryToGameInstance();

			// Play Earn Sound
			if (nullptr != EarnSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), EarnSound, 0.75f);
			}

			


			// Display Parts Info UI
			APPPlayerController* PlayerController = Cast<APPPlayerController>(player->GetController());
			if (PlayerController)
			{
				PlayerController->OpenPartsPauseWidget(TSubclassOf<UUserWidget>(PartsData->PartsPauseUI), PartsData->PartsInfoTextures);
			}

			Destroy();
		}
	}
}

