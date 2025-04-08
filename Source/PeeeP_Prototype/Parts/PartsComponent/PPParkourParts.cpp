// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPParkourParts.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Parts/PartsData/PPParkourPartsData.h"


UPPParkourParts::UPPParkourParts()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxJumpLevel = 3;
	ParkourJumpMultiplier = 1.25f;
	ParkourSpeedMultiplier = 2.0f;

	TimePerChargeLevel = 0.5f;

	static ConstructorHelpers::FObjectFinder<UPPParkourPartsData> ParkourPartsDataRef(TEXT("/Script/PeeeP_Prototype.PPParkourPartsData'/Game/Parts/Parkour/PartkourPartsData.PartkourPartsData'"));
	if (ParkourPartsDataRef.Object)
	{
		PartsData = ParkourPartsDataRef.Object;
	}

}

void UPPParkourParts::OnComponentCreated()
{
	Super::OnComponentCreated();

	Owner = Cast<APPCharacterPlayer>(GetOwner());

	if (IsValid(Owner))
	{
		DefaultJumpZVelocity = Owner->GetCharacterMovement()->JumpZVelocity;
		DefaultMaxWalkSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;

		Owner->GetCharacterMovement()->MaxWalkSpeed *= ParkourSpeedMultiplier;

		APlayerController* PlayerController = CastChecked<APlayerController>(Owner->GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UPPParkourPartsData* ParkourPartsData = Cast<UPPParkourPartsData>(PartsData);
			if (ParkourPartsData)
			{
				Subsystem->AddMappingContext(ParkourPartsData->PartsMappingContext, 1);
				UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent);

				EnhancedInputComponent->BindAction(ParkourPartsData->ChargingJumpAction, ETriggerEvent::Started, this, &UPPParkourParts::ChargStart);
				EnhancedInputComponent->BindAction(ParkourPartsData->ChargingJumpAction, ETriggerEvent::Ongoing, this, &UPPParkourParts::TickJumpCharge);
				EnhancedInputComponent->BindAction(ParkourPartsData->ChargingJumpAction, ETriggerEvent::Completed, this, &UPPParkourParts::Jump);
			}
		}
	}
}

void UPPParkourParts::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UPPParkourParts::BeginPlay()
{
	Super::BeginPlay();
}

void UPPParkourParts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPPParkourParts::CleanUpParts()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	Owner->GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
}

void UPPParkourParts::ChargStart()
{
	bIsIsCharging = true;

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		PreviousJumpChargingTime = World->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}
}

void UPPParkourParts::TickJumpCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("Charge"));
	if (bIsIsCharging)
	{
		CurrentJumpChargingTime = GetWorld()->GetTimeSeconds();

		TotalJumpChargingTime += CurrentJumpChargingTime - PreviousJumpChargingTime;

		TotalJumpChargingTime = FMath::Clamp(TotalJumpChargingTime, 0.0f, TimePerChargeLevel);

		UE_LOG(LogTemp, Warning, TEXT("TotalJumpChargingTime %f"), TotalJumpChargingTime);

		if (TotalJumpChargingTime >= TimePerChargeLevel)
		{
			CurrentJumpLevel = FMath::Clamp(CurrentJumpLevel + 1, 1, MaxJumpLevel);
			TotalJumpChargingTime = 0.0f;
			UE_LOG(LogTemp, Warning, TEXT("Current Jump Level %d"), CurrentJumpLevel);
		}

		PreviousJumpChargingTime = CurrentJumpChargingTime;
	}

}

void UPPParkourParts::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Charge Jump"));

	if (CurrentJumpLevel > 0)
	{
		int32 JumpMultiplierExponent = CurrentJumpLevel - 1;
		float FinalJumpMultiplier = FMath::Pow(ParkourJumpMultiplier, JumpMultiplierExponent);

		Owner->GetCharacterMovement()->JumpZVelocity *= FinalJumpMultiplier;
	}
	
	Owner->Jump();
	bIsIsCharging = false;
}
