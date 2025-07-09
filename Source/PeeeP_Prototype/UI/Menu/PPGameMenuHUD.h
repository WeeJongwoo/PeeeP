// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPGameMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGameMenuHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPGameMenuHUD(const FObjectInitializer& ObjectInitializer);
	
protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_StartButton;

	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_LoadButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_SettingButton;
	
	UPROPERTY(VisibleAnywhere, Category = Button, meta = (BindWidget))
	TObjectPtr<class UPPMenuButtonWidget> WBP_ExitButton;

	UPROPERTY(VisibleAnywhere, Category = Button)
	TObjectPtr<class APPLobbyActor> LobbyActor;

	TSubclassOf<class APPLobbyActor> LobbyActorClass;

	UPROPERTY(VisibleAnywhere, Category = Widget, meta = (BindWidget))
	TObjectPtr<class UPPSettingMenu> WBP_SettingMenu;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> StartButtonHoverStartAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> StartButtonHoverEndAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> LoadButtonHoverStartAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> LoadButtonHoverEndAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingButtonHoverStartAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> SettingButtonHoverEndAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ExitButtonHoverStartAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ExitButtonHoverEndAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> LogoLoopAnim;

protected:

	UFUNCTION()
	void StartButtonClick();

	UFUNCTION()
	void LoadButtonClick();

	UFUNCTION()
	void SettingButtonClick();

	UFUNCTION()
	void ExitButtonClick();

	UFUNCTION()
	void StartButtonHover();

	UFUNCTION()
	void LoadButtonHover();

	UFUNCTION()
	void SettingButtonHover();

	UFUNCTION()
	void ExitButtonHover();

	UFUNCTION()
	void StartButtonHoverEnd();

	UFUNCTION()
	void LoadButtonHoverEnd();

	UFUNCTION()
	void SettingButtonHoverEnd();

	UFUNCTION()
	void ExitButtonHoverEnd();
};
