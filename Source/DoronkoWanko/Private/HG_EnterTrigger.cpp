// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterTrigger.h"
#include "ObjectWidget.h"
#include "HG_EnterInstruction.h"
#include "Components/BoxComponent.h"
#include "GW_Player.h"

// Sets default values
AHG_EnterTrigger::AHG_EnterTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));

	SetRootComponent(BoxComp);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("EnterTrigger"));

	RoomName = FText::FromString(TEXT("Default"));
}

// Called when the game starts or when spawned
void AHG_EnterTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_EnterTrigger::OnMyBeginOverlap);
}

// Called every frame
void AHG_EnterTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_EnterTrigger::EnterTriggered()
{
	Widget = CreateWidget<UHG_EnterInstruction>(GetWorld(), WidgetFactory);
	if (Widget) {
		Widget->AddToViewport();
		Widget->SetText(RoomName);
	}
}

void AHG_EnterTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGW_Player>()) {
		EnterTriggered();
	}
}

