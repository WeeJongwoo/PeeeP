// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPParkourParts.h"
#include "Character/PPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Parts/PartsData/PPParkourPartsData.h"
#include "Components/AudioComponent.h"
#include "Containers/Ticker.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


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

	ParkourSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ParkourSoundComponent"));
	ChargingEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargingEffectComponent"));

}

void UPPParkourParts::OnComponentCreated()
{
	Super::OnComponentCreated();

	Owner = Cast<APPCharacterPlayer>(GetOwner());

	if (IsValid(Owner))
	{
		DefaultJumpZVelocity = Owner->GetCharacterMovement()->JumpZVelocity;
		DefaultMaxWalkSpeed = Owner->GetMaxWalkSpeed();

		Owner->SetMaxWalkSpeed(DefaultMaxWalkSpeed * ParkourSpeedMultiplier);

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

				ChargeSound = ParkourPartsData->ChargeSound;
				JumpSound = ParkourPartsData->JumpSound;
				ChargingEffect = ParkourPartsData->ChargingEffect;
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

	if (ChargingEffect != nullptr && IsValid(ChargingEffectComponent))
	{
		ChargingEffectComponent->SetAutoActivate(false);
		ChargingEffectComponent->SetupAttachment(Owner->GetRootComponent());

		ChargingEffectComponent->SetAsset(ChargingEffect);
		ChargingEffectComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -20.0f), FRotator::ZeroRotator);
		ChargingEffectComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
}

void UPPParkourParts::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPPParkourParts::CleanUpParts()
{
	Super::CleanUpParts();

	Owner->SetMaxWalkSpeed(DefaultMaxWalkSpeed);
	Owner->SetRunning(false);
	ChargingEffectComponent->DestroyComponent();
}

void UPPParkourParts::ChargStart()
{
	bIsCharging = true;
	if (CurrentJumpLevel > 0)
	{
		CurrentJumpLevel = 0;
	}

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		PreviousJumpChargingTime = World->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}
	if (IsValid(ParkourSoundComponent) && IsValid(ChargeSound))
	{
		ParkourSoundComponent->SetSound(ChargeSound);
		ParkourSoundComponent->SetVolumeMultiplier(0.5f);
		ParkourSoundComponent->SetPitchMultiplier(1.0f);
		ParkourSoundComponent->Play();
	}
}

void UPPParkourParts::TickJumpCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("Charge"));
	if (bIsCharging)
	{
		CurrentJumpChargingTime = GetWorld()->GetTimeSeconds();

		TotalJumpChargingTime += CurrentJumpChargingTime - PreviousJumpChargingTime;

		TotalJumpChargingTime = FMath::Clamp(TotalJumpChargingTime, 0.0f, TimePerChargeLevel);

		//UE_LOG(LogTemp, Warning, TEXT("TotalJumpChargingTime %f"), TotalJumpChargingTime);

		if (TotalJumpChargingTime >= TimePerChargeLevel && CurrentJumpLevel < MaxJumpLevel)
		{
			CurrentJumpLevel = FMath::Clamp(CurrentJumpLevel + 1, 1, MaxJumpLevel);
			TotalJumpChargingTime = 0.0f;

			if (ChargingEffectComponent->GetAsset() == ChargingEffect)
			{
				ChargingEffectComponent->Activate(true);
				UE_LOG(LogTemp, Warning, TEXT("ChargingEffectComponent Activate"));
			}
		
			UE_LOG(LogTemp, Warning, TEXT("Current Jump Level %d"), CurrentJumpLevel);
		}

		PreviousJumpChargingTime = CurrentJumpChargingTime;
	}

}

void UPPParkourParts::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Charge Jump"));

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
		{
			if (IsValid(this))
			{
				bIsCharging = false;
			}
		}
	);

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	if (Owner->GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (IsValid(ParkourSoundComponent))
	{
		if (ParkourSoundComponent->IsPlaying())
		{
			ParkourSoundComponent->Stop();
		}
		if (IsValid(JumpSound))
		{
			ParkourSoundComponent->SetSound(JumpSound);
			ParkourSoundComponent->SetVolumeMultiplier(0.5f);
			ParkourSoundComponent->SetPitchMultiplier(1.0f);
			ParkourSoundComponent->Play();
		}
	}

	if (CurrentJumpLevel > 0)
	{
		int32 JumpMultiplierExponent = CurrentJumpLevel - 1;
		float FinalJumpMultiplier = FMath::Pow(ParkourJumpMultiplier, JumpMultiplierExponent);
		float JumpSpeed = DefaultJumpZVelocity * FinalJumpMultiplier;

		Owner->LaunchCharacter(FVector(0.0f, 0.0f, JumpSpeed), true, false);
		UE_LOG(LogTemp, Warning, TEXT("Current Jump Level %d"), CurrentJumpLevel);

		//Owner->GetCharacterMovement()->JumpZVelocity *= FinalJumpMultiplier;
	}
	
	//Owner->Jump();
}
