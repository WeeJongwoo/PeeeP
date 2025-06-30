// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Parts/PPPartsPauseUIBase.h"

void UPPPartsPauseUIBase::End()
{
	if (EndUIDelegate.IsBound())
	{
		EndUIDelegate.Execute();
	}
}
