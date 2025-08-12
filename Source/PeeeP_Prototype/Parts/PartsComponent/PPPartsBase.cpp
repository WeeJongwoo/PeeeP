// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPPartsBase.h"
#include "Parts/PartsData/PPGrabPartsData.h"

#include "Character/PPCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UPPPartsBase::UPPPartsBase()
{
	//항상 true로 설정해야 함. false 설정시, TickFunction의 등록 자체를 막아버리기 때문에,절대로 Tick을 사용하지 않을 경우에만 false로 설정함.
	PrimaryComponentTick.bCanEverTick = true; 

	//Tick을 인위적으로 조작 가능함.
	//PrimaryComponentTick.bStartWithTickEnabled = false;

	Owner = Cast<APPCharacterPlayer>(GetOwner());

}

// Called when the game starts
void UPPPartsBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPPPartsBase::CleanUpParts()
{
	Owner->GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}


// Called every frame
void UPPPartsBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPPPartsBase::PartsInit(TObjectPtr<class UPPPartsDataBase> InPartsData)
{
	if (IsValid(Owner))
	{
		Owner->GetMesh()->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
		if (IsValid(InPartsData))
		{
			PartsData = InPartsData;
			HitAnimMontage = PartsData->HitAnimMontage;
			ConsumptionType = PartsData->ConsumptionType;
			ElectricConsumption = PartsData->ElectricConsumption;
		}
	}
}

void UPPPartsBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Clean up the parts when the component ends play
	CleanUpParts();

	APPCharacterPlayer* PlayerCharacter = Cast<APPCharacterPlayer>(GetOwner());
	if (PlayerCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
		if (!PlayerController)
		{
			return;
		}
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (PartsData)
			{
				Subsystem->RemoveMappingContext(PartsData->PartsMappingContext);
			}
		}
	}
}

void UPPPartsBase::PlayHitAnimation()
{
	if (IsValid(Owner) && IsValid(HitAnimMontage))
	{
		Owner->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("Hit")), HitAnimMontage);
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(HitAnimMontage, 1.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitAnimMontage is not valid!"));
	}
}


