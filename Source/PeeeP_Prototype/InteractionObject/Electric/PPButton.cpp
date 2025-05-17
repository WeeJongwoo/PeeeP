// Fill out your copyright notice in the Description page of Project Settings.


#include "PPButton.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APPButton::APPButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));
	RootComponent = ButtonMesh;

	//ButtonCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollider"));
	//ButtonCollider->SetupAttachment(ButtonMesh);


}

// Called when the game starts or when spawned
void APPButton::BeginPlay()
{
	Super::BeginPlay();

}

//void APPButton::OnButtonPressed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	OnButtonPressedDelegate.ExecuteIfBound();
//}

// Called every frame
void APPButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPButton::Charge()
{
	OnButtonPressedDelegate.ExecuteIfBound();

	if (IsValid(SwitchOnMi01) && IsValid(SwitchOnMi02))
	{
		ButtonMesh->SetMaterial(0, SwitchOnMi01);
		ButtonMesh->SetMaterial(1, SwitchOnMi02);
	}
}

