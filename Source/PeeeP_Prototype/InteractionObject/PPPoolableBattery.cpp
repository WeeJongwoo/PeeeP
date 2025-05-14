﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPPoolableBattery.h"
#include "Containers/PPPoolObject.h"

APPPoolableBattery::APPPoolableBattery()
{
}

void APPPoolableBattery::SetObjectPool(UPPPoolObject* InObjedtPool)
{
	ObjectPool = InObjedtPool;
}

void APPPoolableBattery::ReturnSelf()
{
	if (ObjectPool != nullptr)
	{
		ObjectPool->ReturnObject(this);
		Mesh->SetSimulatePhysics(false);
	}
	this->SetActive(false);
}

void APPPoolableBattery::SetActive(bool IsActive)
{
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	Mesh->SetSimulatePhysics(true);
	this->SetActorHiddenInGame(!IsActive);
}
