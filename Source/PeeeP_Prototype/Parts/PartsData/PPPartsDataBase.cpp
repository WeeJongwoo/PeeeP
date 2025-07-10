// Fill out your copyright notice in the Description page of Project Settings.


#include "Parts/PartsData/PPPartsDataBase.h"

UPPPartsDataBase::UPPPartsDataBase()
{
	ConstructorHelpers::FClassFinder<UPPPartsPauseUIBase> PartsUIClassRef(TEXT("/Game/UI/Parts/WBP_PartsUI.WBP_PartsUI_C"));
	if(PartsUIClassRef.Class)
	{
		PartsPauseUI = PartsUIClassRef.Class;
	}
}

FPrimaryAssetId UPPPartsDataBase::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("PPPartsData", GetFName());
}
