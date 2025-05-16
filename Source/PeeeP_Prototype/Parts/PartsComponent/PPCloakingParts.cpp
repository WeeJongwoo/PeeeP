// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPCloakingParts.h"
#include "Character/PPCharacterPlayer.h"
#include "Parts/PartsData/PPCloakingPartsData.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"

UPPCloakingParts::UPPCloakingParts()
{
	static ConstructorHelpers::FObjectFinder<UPPCloakingPartsData> CloakingPartsDataRef(TEXT("/Script/PeeeP_Prototype.PPCloakingPartsData'/Game/Parts/Cloaking/CloakingPartsData.CloakingPartsData'"));
	if (CloakingPartsDataRef.Object)
	{
		PartsData = CloakingPartsDataRef.Object;
	}

	CloakingSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CloakingSoundComponent"));

	bCanCloaking = true;
}

void UPPCloakingParts::OnComponentCreated()
{
	Super::OnComponentCreated();

	Owner = Cast<APPCharacterPlayer>(GetOwner());

	//Setup
	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(Owner);
	if (PlayerCharacter)
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(PlayerCharacter->GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UPPCloakingPartsData* CloakingPartsData = Cast<UPPCloakingPartsData>(PartsData);
			if (CloakingPartsData)
			{
				Subsystem->AddMappingContext(CloakingPartsData->PartsMappingContext, 1);
				UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

				EnhancedInputComponent->BindAction(CloakingPartsData->CloakingAction, ETriggerEvent::Started, this, &UPPCloakingParts::StartCloaking);

				CloakingTime = CloakingPartsData->CloakingTime;

				CloakingMaterial = CloakingPartsData->CloakingMaterial;
				NumMaterials = CloakingPartsData->PartsMesh->GetNumMaterials();
				Cooldown = CloakingPartsData->Cooldown;

				StartSound = CloakingPartsData->StartSound;
				EndSound = CloakingPartsData->EndSound;
			}
		}
	}
}

void UPPCloakingParts::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UPPCloakingParts::BeginPlay()
{
	Super::BeginPlay();

}

void UPPCloakingParts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPPCloakingParts::CleanUpParts()
{
	Super::CleanUpParts();

	StopCloaking();

	GetWorld()->GetTimerManager().ClearTimer(CloakingCooldownTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(CloakingTimerHandle);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (IsValid(CloakingSoundComponent) && CloakingSoundComponent->IsPlaying())
	{
		CloakingSoundComponent->Stop();
	}
}

void UPPCloakingParts::StartCloaking()
{
	if (!bCanCloaking)
	{
		return;
	}

	if (IsValid(Owner))
	{
		if (NumMaterials > 0 && IsValid(CloakingMaterial))
		{
			for (int32 i = 0; i < NumMaterials; i++)
			{
				Owner->GetMesh()->SetMaterial(i, CloakingMaterial);
			}
			UE_LOG(LogTemp, Log, TEXT("CloakingMaterial Set"));
		}

		bCanCloaking = false;

		Owner->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel11, ECR_Ignore);

		GetWorld()->GetTimerManager().SetTimer(CloakingTimerHandle, this, &UPPCloakingParts::StopCloaking, CloakingTime, false);

		if (IsValid(CloakingSoundComponent) && IsValid(StartSound))
		{
			CloakingSoundComponent->SetSound(StartSound);
			CloakingSoundComponent->SetVolumeMultiplier(0.5f);
			CloakingSoundComponent->SetPitchMultiplier(1.0f);
			CloakingSoundComponent->Play();
		}
	}
}

void UPPCloakingParts::StopCloaking()
{
	if (IsValid(Owner))
	{
		UE_LOG(LogTemp, Log, TEXT("DefaultMaterial Set"));
		if (NumMaterials > 0 && IsValid(CloakingMaterial))
		{
			for (int i = 0; i < NumMaterials; i++)
			{
				Owner->GetMesh()->SetMaterial(i, nullptr);
			}

			UE_LOG(LogTemp, Log, TEXT("DefaultMaterial Set"));

			Owner->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel11, ECR_Overlap);

			GetWorld()->GetTimerManager().SetTimer(CloakingCooldownTimerHandle, this, &UPPCloakingParts::SetCloakingState, Cooldown, false);

			if (IsValid(CloakingSoundComponent) && IsValid(EndSound))
			{
				if (CloakingSoundComponent->IsPlaying())
				{
					CloakingSoundComponent->Stop();
				}
				CloakingSoundComponent->SetSound(EndSound);
				CloakingSoundComponent->SetVolumeMultiplier(0.5f);
				CloakingSoundComponent->SetPitchMultiplier(1.0f);
				CloakingSoundComponent->Play();
			}
		}
	}
}

void UPPCloakingParts::SetCloakingState()
{
	bCanCloaking = true;
}
