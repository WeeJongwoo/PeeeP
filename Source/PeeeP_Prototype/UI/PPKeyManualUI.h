// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPKeyManualUI.generated.h"

UENUM(BlueprintType)
enum class EKeyManual : uint8
{
	VISIBLE_TIMEOUT UMETA(DisplayName = "Visible Timeout"),
	NONE UMETA(DisplayName = "None"),
	MOVE UMETA(DisplayName = "Move"),
	JUMP UMETA(DisplayName = "Jump"),
	CHARGE UMETA(DisplayName = "Charge"),
	DISCHARGE UMETA(DisplayName = "Discharge"),
	EQUIP UMETA(DisplayName = "Equip"),
	UNEQUIP UMETA(DisplayName = "Unequip"),
	GRAB UMETA(DisplayName = "Grab"),
	GRAB_RELEASE UMETA(DisplayName = "Grab Release"),
};

UCLASS()
class PEEEP_PROTOTYPE_API UPPKeyManualUI : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct() override;

protected:

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualMove;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualJump;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualCharge;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualDischarge;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualEquip;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualUnequip;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualGrab;

	UPROPERTY()
	TObjectPtr <class UUserWidget> KeyManualGrabRelease;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> MoveUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> JumpUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> ChargeUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> DischargeUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> EquipUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> UnequipUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> GrabUIAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr <class UWidgetAnimation> GrabReleaseUIAnim;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyManuals")
	uint8 bIsKeyManualVisible : 1;

private:
	void StopAnimations();

	void StartVisibleKeyManual(EKeyManual KeyManual);

public:
	void SetAllKeyManualHidden();
	void SetKeyManualVisible(uint8 KeyManual);

	uint8 GetIsKeyManualVisible() const;
	
};
