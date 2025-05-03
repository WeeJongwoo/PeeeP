// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPCloakingParts.h"
#include "Character/PPCharacterPlayer.h"
#include "Components/CapsuleComponent.h"

UPPCloakingParts::UPPCloakingParts()
{
}

void UPPCloakingParts::OnComponentCreated()
{
	Super::OnComponentCreated();
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
}

void UPPCloakingParts::StartCloaking()
{
	if (IsValid(Owner))
	{
		if (IsValid(DefaultMaterial) && IsValid(CloakingMaterial))
		{
			Owner->GetMesh()->SetMaterial(0, CloakingMaterial);
		}

		Owner->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel11, ECR_Ignore);
	}
}

void UPPCloakingParts::StopCloaking()
{
	if (IsValid(Owner))
	{
		if (IsValid(DefaultMaterial) && IsValid(CloakingMaterial))
		{
			Owner->GetMesh()->SetMaterial(0, DefaultMaterial);
		}
		Owner->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel11, ECR_Overlap);
	}
}
