// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsComponent/PPCloakingParts.h"

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
}

void UPPCloakingParts::StopCloaking()
{
}
