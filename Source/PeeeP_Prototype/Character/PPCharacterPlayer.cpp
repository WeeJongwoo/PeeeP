﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PPCharacterControlData.h"
#include "Interface/PPInteractableObjectInterface.h"
#include "Parts/PartsComponent/PPGrabParts.h"
#include "Parts/PartsData/PPPartsDataBase.h"
#include "Component/PPElectricDischargeComponent.h"
#include "GameMode/PPPlayerController.h"
//#include "Animation/AnimMontage.h"
#include "NiagaraComponent.h"
#include "Prop/PPCleaningRobot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameMode/PPGameModeBase.h"
#include "GameMode/PPPlayerState.h"
#include "Components/AudioComponent.h"
#include "Animation/PPAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/PPChargingLevelHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PPGameInstance.h"

APPCharacterPlayer::APPCharacterPlayer()
{
	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Run.IA_Run'"));
	if (!InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ButtonInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Button.IA_Button'"));
	if (!ButtonInteractRef.Object)
	{
		ButtonInteract = ButtonInteractRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ElectricDischargeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Discharge.IA_Discharge'"));
	if (!ElectricDischargeActionRef.Object)
	{
		ElectricDischargeAction = ElectricDischargeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ElectricDischargeModeChangeActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_DischargeModeChange.IA_DischargeModeChange'"));
	if (!ElectricDischargeModeChangeActionRef.Object)
	{
		ElectricDischargeModeChangeAction = ElectricDischargeModeChangeActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> OpenMenuActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OpenMenu.IA_OpenMenu'"));
	if (!OpenMenuActionRef.Object)
	{
		OpenMenuInteract = OpenMenuActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RespawnTestInputActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/PeeeP_Sequence/IA_Restart.IA_Restart'"));
	if (!RespawnTestInputActionRef.Object)
	{
		RespawnTestInputAction = RespawnTestInputActionRef.Object;
	}

	// Quick Slot Section
	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_QuickSlotMove.IA_QuickSlotMove'"));
	if (!QuickSlotMoveRef.Object)
	{
		QuickSlotMoveAction = QuickSlotMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotUseRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_QuickSlotUse.IA_QuickSlotUse'"));
	if (!QuickSlotUseRef.Object)
	{
		QuickSlotUseAction = QuickSlotUseRef.Object;
	}
	bIsAllowWheelInput = true;

	// Default Mesh & Animation Setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PeeP_Characters/new_beep/new_beep_rig.new_beep_rig'"));
	if (MeshRef.Object)
	{
		DefaultSkeletonMesh = MeshRef.Object;
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprintRef(TEXT("/Game/PeeP_Characters/new_beep/ABP_PeeeP.ABP_PeeeP_C"));
	if (AnimBlueprintRef.Class)
	{
		DefaultAnimClass = AnimBlueprintRef.Class;
		GetMesh()->SetAnimInstanceClass(AnimBlueprintRef.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));

	RunningMultiplier = 2.0f;	// Running Speed Multiplier. You can Setting Running Speed Multiplier.

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// CameraBoom 컴포?��?���? �??��?��
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 100.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;
	CameraBoom->CameraRotationLagSpeed = 20.0f;
	CameraBoom->CameraLagMaxDistance = 300.f;
	CameraBoom->ProbeSize = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));	// FollowCamera 컴포?��?���? �??��?��
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	DefaultFOV = 90.0f;
	MaxRunningFOV = 105.0f;
	FollowCamera->FieldOfView = DefaultFOV;

	ElectricDischargeComponent = CreateDefaultSubobject<UPPElectricDischargeComponent>(TEXT("ElectricDischargeComponent"));

	// Player Movement Setting
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.6f;
	DefaultMaxWalkSpeed = 100.0f;
	this->MaxWalkSpeed = DefaultMaxWalkSpeed;										// Setting Default Max Walk Speed
	GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;		// Apply Default Max Walk Speed
	GetCharacterMovement()->MaxStepHeight = 5.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	this->bIsRunning = false;

	ElectricNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ElectricEffectComponent"));
	ElectricNiagaraComponent->SetupAttachment(RootComponent);

	PlayerEffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PlayerEffectComponent"));
	PlayerEffectNiagaraComponent->SetupAttachment(RootComponent);

	// ?��벤토�? 컴포?��?��
	InventoryComponent = CreateDefaultSubobject<UPPInventoryComponent>(TEXT("InventoryComponent"));

	AttachedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachedMesh"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APPCharacterPlayer::OnBeginOverlap);

	// Electric Charging Level Widget
	ElectricChargingLevelWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	ElectricChargingLevelWidgetComponent->SetupAttachment(GetMesh());
	ElectricChargingLevelWidgetComponent->SetRelativeLocation(FVector{ 0.0f, 0.0f, 24.0f });

	static ConstructorHelpers::FClassFinder<UPPChargingLevelHUD> ElectricChargingLevelWidgetComponentRef = TEXT("/Game/UI/PlayerStatus/Charging/WB_ChargingLevelHUD.WB_ChargingLevelHUD_C");
	if (ElectricChargingLevelWidgetComponentRef.Class)
	{
		ElectricChargingLevelWidgetComponent->SetWidgetClass(ElectricChargingLevelWidgetComponentRef.Class);
		ElectricChargingLevelWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		ElectricChargingLevelWidgetComponent->SetDrawSize(FVector2D{ 256.0f, 128.0f });
		ElectricChargingLevelWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


}

void APPCharacterPlayer::OnDeath(uint8 bIsDead)
{
	SetDefaultMeshAndAnim();

	UPPAnimInstance* AnimInstance = Cast<UPPAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		if (AnimInstance->Montage_IsPlaying(HitAnimMontage))
		{
			AnimInstance->Montage_Stop(0, HitAnimMontage);
		}

		AnimInstance->SetbIsDead(bIsDead);
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (bIsDead)
		{
			if (!GetWorldTimerManager().IsTimerActive(RespawnTimerHandle))
			{
				InventoryComponent->ClearUsingItem();
				ElectricDischargeComponent->CancelCharging();
				RemoveParts();
				PlayEquipEffect();
				PlayDeadSound();

				if (IsValid(PlayerController))
				{
					this->DisableInput(PlayerController);
				}
				GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APPCharacterPlayer::PlayRespawnSequence, 2.25f, false);
				
			}
		}
	}
	
}

void APPCharacterPlayer::PlayRespawnSequence()
{
	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		ElectricDischargeComponent->Reset();
		GameMode->MoveCharacterToSpawnLocation(this);
		OnDeath(false);	// Set bIsDead in AnimInstance
		GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	}
}

void APPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	APPGameModeBase* GameMode = Cast<APPGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		GameMode->SetInitialSpawnLocation(PlayerController);
	}

	ElectricChargingLevelWidget = Cast<UPPChargingLevelHUD>(ElectricChargingLevelWidgetComponent->GetWidget());
	if (IsValid(ElectricChargingLevelWidget))
	{
		ElectricChargingLevelWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	DeadEventDelegate.BindUObject(this, &APPCharacterPlayer::OnDeath);

	//Test_EquipGrabParts();

	//// Parts ?��?���? ?��?��?��?��?�� �??��?��
	//// ?��?�� �?분�?? ?��중에 ?��벤토리에?�� ?��?��?�� ?��?��?��?�� ?���? �?경하?�� ?��?�� ?���? 만들?��?�� ?��?��?���? ?�� ?��
	//UActorComponent* PartsComponent = AddComponentByClass(UPPGrabParts::StaticClass(), true, FTransform::Identity, false);
	//Parts = CastChecked<UPPPartsBase>(PartsComponent);
}

void APPCharacterPlayer::Tick(float DeltaTime)
{
	//Idle?��?��?��?��?�� ?��?��?��?���? ???직이?�� actor????�� 충돌?�� 무시?���? ?��버려 ?��?�� 코드�? 추�???��.
	FHitResult OutHit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.03f), GetActorRotation(), true, OutHit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.03f), GetActorRotation(), true, OutHit);

	SetFOVBySpeed(DeltaTime);
}

void APPCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	/// Binding Section
	/// If you added new action, you should add new binding action here.
	/// Create some function for actions about added input and bind it.
	
	// Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	// Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Move);
	// Look
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::Look);
	// Running
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OnRunningStart);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPCharacterPlayer::OnRunningEnd);
	// Button Interaction(Unused)
	EnhancedInputComponent->BindAction(ButtonInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::ButtonInteraction);
	// Open Menu
	EnhancedInputComponent->BindAction(OpenMenuInteract, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OpenMenu);

	//EnhancedInputComponent->BindAction(RespawnTestInputAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::OnDeath); // 테스트용

	// Electric Discharge Component
	if (ElectricDischargeComponent && ElectricChargingLevelWidgetComponent)
	{
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Ongoing, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Charging);
		EnhancedInputComponent->BindAction(ElectricDischargeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::Discharge);
		EnhancedInputComponent->BindAction(ElectricDischargeModeChangeAction, ETriggerEvent::Completed, ElectricDischargeComponent.Get(), &UPPElectricDischargeComponent::ChangeDischargeMode);
	}
	// Quick Slot
	EnhancedInputComponent->BindAction(QuickSlotMoveAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotMove);
	EnhancedInputComponent->BindAction(QuickSlotUseAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::QuickSlotUse);

	EnhancedInputComponent->BindAction(RespawnTestInputAction, ETriggerEvent::Triggered, this, &APPCharacterPlayer::SetElectricCapacity, 0.0f);
}

void APPCharacterPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APPCleaningRobot* CleaingRobotRef = Cast<APPCleaningRobot>(OtherActor))
	{
		FRotator Rotation = CleaingRobotRef->GetActorRotation();
		FVector KnockbackVelocity = UKismetMathLibrary::GetForwardVector(Rotation) * CleaingRobotRef->KnockbackStrength;

		// ?��?��?��?��?���? ?���? ?��?��
		LaunchCharacter(KnockbackVelocity, true, true);
		ElectricDischargeComponent->AddCurrentCapacity(CleaingRobotRef->ElectricLossRate);
	}
}

void APPCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UPPCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void APPCharacterPlayer::SetCharacterControlData(const UPPCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void APPCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APPCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APPCharacterPlayer::ButtonInteraction(const FInputActionValue& Value)
{
	FVector CameraPos = FollowCamera->GetComponentLocation();
	FVector CameraForwardVector = FollowCamera->GetForwardVector();
	FVector EndPos = CameraPos + CameraForwardVector * 600.f;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParam(SCENE_QUERY_STAT(Button), false, this);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraPos, EndPos, ECC_GameTraceChannel1, CollisionParam, FCollisionResponseParams(ECR_Block));

	if (IsHit)
	{
		AActor* HitActor = HitResult.GetActor();
		IPPInteractableObjectInterface* ButtonActor = Cast<IPPInteractableObjectInterface>(HitActor);
		//ensure(ButtonActor);
		if (ButtonActor != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("FindButton"));
			ButtonActor->Interact();
		}

	}

	FColor DebugColor(255, 0, 0);

	DrawDebugLine(GetWorld(), CameraPos, EndPos, DebugColor, false, 5.0f);
}

void APPCharacterPlayer::OnRunningStart(const FInputActionValue& Value)
{
	// Set Player Max Walk Speed for Running.
	if (!this->bIsRunning)
	{
		float RunnigSpeed = this->MaxWalkSpeed* RunningMultiplier;
		// Here is Running Max Walk Speed. You can Setting Running Max Walk Speed.
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if (IsValid(Movement))
		{
			Movement->MaxWalkSpeed = RunnigSpeed;
		}
		this->bIsRunning = true;
		UE_LOG(LogTemp, Log, TEXT("Running Start"));
	}
}

void APPCharacterPlayer::OnRunningEnd(const FInputActionValue& Value)
{
	// Set Player Max Walk Speed to Default Max Walk Speed.
	GetCharacterMovement()->MaxWalkSpeed /= RunningMultiplier;
	this->bIsRunning = false;
	UE_LOG(LogTemp, Log, TEXT("Running End"));
}

void APPCharacterPlayer::SetFOVBySpeed(float DeltaTime)
{
	double Speed2D = FMath::Clamp(GetCharacterMovement()->Velocity.Size2D(), DefaultMaxWalkSpeed, MaxWalkSpeed * RunningMultiplier);

	double SpeedRatio = (Speed2D - DefaultMaxWalkSpeed) / ((DefaultMaxWalkSpeed * RunningMultiplier * 2) - DefaultMaxWalkSpeed);

	float TargetFOV = FMath::Lerp(DefaultFOV, MaxRunningFOV, SpeedRatio);

	float SmoothSpeed = 5.0f;
	float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, SmoothSpeed);

	FollowCamera->SetFieldOfView(NewFOV);
}

void APPCharacterPlayer::SetMaxWalkSpeed(float InMaxWalkSpeed)
{
	MaxWalkSpeed = InMaxWalkSpeed;
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed * RunningMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void APPCharacterPlayer::SetRunning(bool InIsRunning)
{
	bIsRunning = InIsRunning;
}

void APPCharacterPlayer::SetDefaultMeshAndAnim()
{
	if (GetMesh()->GetSkeletalMeshAsset() != DefaultSkeletonMesh)
	{
		GetMesh()->SetSkeletalMesh(DefaultSkeletonMesh);
	}

	if (GetMesh()->GetAnimClass() != DefaultAnimClass)
	{
		GetMesh()->SetAnimInstanceClass(DefaultAnimClass);
	}
}

UCameraComponent* APPCharacterPlayer::GetCamera()
{
	return FollowCamera;
}


void APPCharacterPlayer::SwitchParts(UPPPartsDataBase* InPartsData)
{
	RemoveParts();

	UE_LOG(LogTemp, Log, TEXT("Creat New Parts"));
	UActorComponent* PartsComponent = AddComponentByClass(InPartsData->PartsComponentClass, true, FTransform::Identity, false);
	Parts = CastChecked<UPPPartsBase>(PartsComponent);
	if (Parts)
	{
		Parts->PartsInit(InPartsData);
		GetMesh()->SetSkeletalMesh(InPartsData->PartsMesh);
		GetMesh()->SetAnimClass(InPartsData->AnimClass);

		int32 MaterialCount = GetMesh()->GetNumMaterials();
		for (int i = 0; i < MaterialCount; i++)
		{
			GetMesh()->SetMaterial(i, nullptr);
		}
	}
}

void APPCharacterPlayer::RemoveParts()
{
    if (Parts)
    {
        UE_LOG(LogTemp, Log, TEXT("Destroy Parts"));
        Parts->DestroyComponent();
		Parts = nullptr;
    }
}

void APPCharacterPlayer::Test_EquipGrabParts()
{

	UActorComponent* PartsComponent = AddComponentByClass(UPPGrabParts::StaticClass(), true, FTransform::Identity, false);
	Parts = CastChecked<UPPPartsBase>(PartsComponent);

	if (Parts)
	{
		GetMesh()->SetSkeletalMesh(Parts->GetPartsData()->PartsMesh);
		GetMesh()->SetAnimClass(Parts->GetPartsData()->AnimClass);
	}
}

void APPCharacterPlayer::PlayAnimation(UAnimMontage* InAnimMontage)
{
	UAnimMontage* CurrentMontage = GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
	if (CurrentMontage == nullptr)
	{
		PlayAnimMontage(InAnimMontage);
	}
}

//그랩 ?��?��메이?�� ?��?�� ?��, Notify�? ?��?�� ?��출됨. 그랩?�� ?��??? ?��브젝?���? ?��?���? 체크. 
void APPCharacterPlayer::GrabHitCheck()
{
	UPPGrabParts* GrabParts = Cast<UPPGrabParts>(Parts);
	if (GrabParts == nullptr) return;

	GrabParts->Grab();
}



void APPCharacterPlayer::ReduationMaxWalkSpeedRatio(float InReductionRatio)
{
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed* InReductionRatio* RunningMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed * InReductionRatio;
	}
	//GetCharacterMovement()->MaxWalkSpeed *= InReductionRatio;
}

void APPCharacterPlayer::RevertMaxWalkSpeed()
{
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed * RunningMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
	}

	//GetCharacterMovement()->MaxWalkSpeed = this->MaxWalkSpeed;
}

UNiagaraComponent* APPCharacterPlayer::GetElectricNiagaraComponent() const
{
	return ElectricNiagaraComponent;
}

UNiagaraComponent* APPCharacterPlayer::GetPlayerEffectNiagaraComponent() const
{
	return PlayerEffectNiagaraComponent;
}

UPPInventoryComponent* APPCharacterPlayer::GetInventoryComponent()
{
	return InventoryComponent;
}

void APPCharacterPlayer::QuickSlotMove(const FInputActionValue& Value)
{
	// 마우?�� ?�� ?��: ?��?��, 마우?�� ?�� ?��?��: ?��?��
	float MoveDir = Value.Get<float>();

	// 마우?�� ?�� ?��?��?�� ?��?�� 방�??
	float InputInterval = 0.1f;	// ?��?�� ?��?��까�?? 간격
	if (bIsAllowWheelInput)
	{
		UE_LOG(LogTemp, Log, TEXT("Quick Slot Move: %f"), MoveDir);
		bIsAllowWheelInput = false;
		GetWorld()->GetTimerManager().SetTimer(QuickSlotMoveTimer, [&]() {bIsAllowWheelInput = true; }, InputInterval, false);

		// ?���? ???직임 구현�?
		// ?���??�� 처음?��?�� ?��?�� ?��?��?��?�� 경우 ?��?��?��?�� ?��?���?�? ?���?�? ?��?��?�� ???직이�? ?��?��?�� ?��.
		if (MoveDir < 0)		// Wheel Down
		{
			InventoryComponent->ModifyCurrentSlotIndex(1);
		}
		else if (MoveDir > 0)	// Wheel Up
		{
			InventoryComponent->ModifyCurrentSlotIndex(-1);
		}
	}
}

void APPCharacterPlayer::QuickSlotUse(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Quick Slot Used!"));

	// 선택된 슬롯 사용부
	// 미장착 상태에서는 어떠한 파츠(현재 보유하고 있는)를 선택하더라도 정상 착용이 되어야 함.
	// 이미 장착하고 있는 파츠를 재선택 할 경우 장착하고 있는 파츠를 해제해야 함.
	// 파츠를 장착하기 위해서는 현재 슬롯의 인덱스를 알아야 가능
	// 테스트 부분: 슬롯 인덱스 0번의 파츠 슬롯 타입의 아이템을 사용해 주세요.
	// 결과: 로그 정상 출력
	//InventoryComponent->UseItem(0, ESlotType::ST_InventoryParts);

	// ?��?�� ?��?��?�� ?���??�� 기반?���? ?��?�� ?��?��?�� ?��?��
	ElectricDischargeComponent->CancelCharging();

	InventoryComponent->UseItemCurrentIndex(ESlotType::ST_InventoryParts);

}

void APPCharacterPlayer::SetWheelInputAllow(bool Value)
{
	bIsAllowWheelInput = Value;
}

void APPCharacterPlayer::PlayEquipEffect()
{
	if (nullptr != EquipmentEffect)
	{
		if (PlayerEffectNiagaraComponent->GetAsset() != EquipmentEffect)
		{
			PlayerEffectNiagaraComponent->SetAsset(EquipmentEffect);
			PlayerEffectNiagaraComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		}
		PlayerEffectNiagaraComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		PlayerEffectNiagaraComponent->Activate(true);
	}

	if (nullptr != EquipmentSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EquipmentSound, 1.0f, 1.0f);
	}
}

void APPCharacterPlayer::PlayDeadSound()
{
	if (nullptr != DeadSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DeadSound);
	}
}

void APPCharacterPlayer::TakeDamage(float Amount)
{
	if (IsValid(Parts))
	{
		Parts->PlayHitAnimation();
	}
	else
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitAnimMontage, 1.0f);
	}
	ElectricDischargeComponent->AddCurrentCapacity(-Amount);
	
}

void APPCharacterPlayer::SetElectricCapacity(float Amount)
{
	ElectricDischargeComponent->SetCurrentCapacity(Amount);
}

void APPCharacterPlayer::OpenMenu()
{
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->OpenMenu();
	}
}

UPPElectricDischargeComponent* APPCharacterPlayer::GetElectricDischargeComponent()
{
	return ElectricDischargeComponent;

}

UWidgetComponent* APPCharacterPlayer::GetElectricChargingLevelWidgetComponent()
{
	return ElectricChargingLevelWidgetComponent;
}

UPPChargingLevelHUD* APPCharacterPlayer::GetElectricChargingLevelWidget()
{
	return ElectricChargingLevelWidget;
}





