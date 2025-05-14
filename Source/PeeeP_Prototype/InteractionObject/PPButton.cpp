// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/PPButton.h"
#include "Components/BoxComponent.h"

// Sets default values
APPButton::APPButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	RootComponent = ButtonMesh;

	ButtonCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollider"));
	ButtonCollider->SetupAttachment(ButtonMesh);


}

// Called when the game starts or when spawned
void APPButton::BeginPlay()
{
	Super::BeginPlay();
	
	ButtonCollider->OnComponentBeginOverlap.AddDynamic(this, &APPButton::OnButtonPressed);
}

void APPButton::OnButtonPressed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnButtonPressedDelegate.ExecuteIfBound();
}

// Called every frame
void APPButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

