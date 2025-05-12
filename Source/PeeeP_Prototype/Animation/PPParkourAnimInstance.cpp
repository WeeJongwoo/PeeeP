// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPParkourAnimInstance.h"
#include "Character/PPCharacterPlayer.h"
#include "Parts/PartsComponent/PPParkourParts.h"

UPPParkourAnimInstance::UPPParkourAnimInstance()
{
}

void UPPParkourAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	APPCharacterPlayer* TempPlayer = Cast<APPCharacterPlayer>(Owner);
	if (nullptr != TempPlayer)
	{
		CharacterPlayer = TempPlayer;
	}
}

void UPPParkourAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(CharacterPlayer))
	{
		UPPParkourParts* ParkurParts = Cast<UPPParkourParts>(CharacterPlayer->GetParts());
		if (IsValid(ParkurParts))
		{
			bIsCharging = ParkurParts->GetIsCharging();
		}
	}

}
