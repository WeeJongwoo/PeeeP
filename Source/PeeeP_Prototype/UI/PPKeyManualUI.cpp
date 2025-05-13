// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPKeyManualUI.h"
#include "Components/Image.h"
#include "Interface/UI/PPElectricHUDInterface.h"

void UPPKeyManualUI::NativeConstruct()
{
	Super::NativeConstruct();

	KeyManualMove = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideMove")));
	KeyManualJump = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideJump")));
	KeyManualCharge = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideElectricCharge")));
	KeyManualDischarge = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideElectricDischarge")));
	KeyManualEquip = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideEquip")));
	KeyManualUnequip = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideUnequip")));
	KeyManualGrab = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideGrab")));
	KeyManualGrabRelease = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBKeyGuideGrabRelease")));

	KeyManualMove->SetVisibility(ESlateVisibility::Hidden);
	KeyManualJump->SetVisibility(ESlateVisibility::Hidden);
	KeyManualCharge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualDischarge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualEquip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualUnequip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualGrab->SetVisibility(ESlateVisibility::Hidden);
	KeyManualGrabRelease->SetVisibility(ESlateVisibility::Hidden);

	// 델리게이트를 위한 인터페이스(플레이어에게 상속됨)
	IPPElectricHUDInterface* ElectricHUDInterface = Cast<IPPElectricHUDInterface>(GetOwningPlayerPawn());
	if (ElectricHUDInterface)
	{
		ElectricHUDInterface->ShowKeyManualDelegate.AddUObject(this, &UPPKeyManualUI::SetKeyManualVisible);
	}

	bIsKeyManualVisible = false;
}

void UPPKeyManualUI::StopAnimations()
{
	if (IsAnyAnimationPlaying())
	{
		StopAllAnimations();
	}
}

void UPPKeyManualUI::StartVisibleKeyManual(EKeyManual KeyManual)
{
	UUserWidget* Widget = nullptr;
	UWidgetAnimation* Anim = nullptr;

	switch (KeyManual)
	{
	case EKeyManual::VISIBLE_TIMEOUT:
		break;
	case EKeyManual::NONE:
		break;
	case EKeyManual::MOVE:
		Widget = KeyManualMove;
		Anim = MoveUIAnim;
		break;
	case EKeyManual::JUMP:
		Widget = KeyManualJump;
		Anim = JumpUIAnim;
		break;
	case EKeyManual::CHARGE:
		Widget = KeyManualCharge;
		Anim = ChargeUIAnim;
		break;
	case EKeyManual::DISCHARGE:
		Widget = KeyManualDischarge;
		Anim = DischargeUIAnim;
		break;
	case EKeyManual::EQUIP:
		Widget = KeyManualEquip;
		Anim = EquipUIAnim;
		break;
	case EKeyManual::UNEQUIP:
		Widget = KeyManualUnequip;
		Anim = UnequipUIAnim;
		break;
	case EKeyManual::GRAB:
		Widget = KeyManualGrab;
		Anim = GrabUIAnim;
		break;
	case EKeyManual::GRAB_RELEASE:
		Widget = KeyManualGrabRelease;
		Anim = GrabReleaseUIAnim;
		break;
	default:
		break;
	}

	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsAnyAnimationPlaying())
	{
		StopAllAnimations();
	}

	if (Anim)
	{
		PlayAnimation(Anim);
	}
}

void UPPKeyManualUI::SetAllKeyManualHidden()
{
	KeyManualMove->SetVisibility(ESlateVisibility::Hidden);
	KeyManualJump->SetVisibility(ESlateVisibility::Hidden);
	KeyManualCharge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualDischarge->SetVisibility(ESlateVisibility::Hidden);
	KeyManualEquip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualUnequip->SetVisibility(ESlateVisibility::Hidden);
	KeyManualGrab->SetVisibility(ESlateVisibility::Hidden);
	KeyManualGrabRelease->SetVisibility(ESlateVisibility::Hidden);

	bIsKeyManualVisible = false;
}

void UPPKeyManualUI::SetKeyManualVisible(uint8 KeyManual)
{
	EKeyManual NewKeyManual = static_cast<EKeyManual>(KeyManual);
	switch (NewKeyManual)
	{
	case EKeyManual::VISIBLE_TIMEOUT:
		if (!IsAnyAnimationPlaying())
		{
			SetAllKeyManualHidden();
		}
		break;
	case EKeyManual::NONE:
		SetAllKeyManualHidden();
		break;
	case EKeyManual::MOVE:
	case EKeyManual::JUMP:
	case EKeyManual::CHARGE:
	case EKeyManual::DISCHARGE:
	case EKeyManual::EQUIP:
	case EKeyManual::UNEQUIP:
	case EKeyManual::GRAB:
	case EKeyManual::GRAB_RELEASE:
		SetAllKeyManualHidden();
		StartVisibleKeyManual(NewKeyManual);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("UPPKeyManualUI::SetKeyManualVisible - Invalid KeyManual"));
		break;
	}

	if (NewKeyManual != EKeyManual::NONE)
	{
		bIsKeyManualVisible = true;
	}
}

uint8 UPPKeyManualUI::GetIsKeyManualVisible() const
{
	return bIsKeyManualVisible;
}


/// <summary>
/// 키 가이드 UI 표시, 입력한 index에 해당하는 하나의 UI만 표시
/// ///
/// 0: 모두 비활성화
/// 1: 기본 조작, 2: 점프, 3: 전기 충전, 4: 전기 방출
/// 5: 파츠 장착, 6: 파츠 해제, 7: 그랩 파츠(그랩), 8: 그랩 파츠(놓기)
/// </summary>
/// <param name="index">표시할 UI</param>
