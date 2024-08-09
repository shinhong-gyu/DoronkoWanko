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

	/*BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("EnterTrigger"));*/

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
	/*auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player) {
		if (RoomSelf != Player->LocState) {
			Player->SetLocState(RoomSelf);
			if (Player->EnterWidget == nullptr) {
				UE_LOG(LogTemp,Warning,TEXT("Player->EnterWidget == nullptr"));
				Player->EnterWidget = CreateWidget<UHG_EnterInstruction>(GetWorld(), Player->WidgetFactory);
				if (Player->EnterWidget) {
					UE_LOG(LogTemp, Warning, TEXT("Player->EnterWidget"));
					Player->EnterWidget->AddToViewport();
					Player->EnterWidget->SetText(RoomName);
				}
			}
			else {
				Player->EnterWidget->RemoveFromParent();
				Player->EnterWidget = nullptr;
				EnterTriggered();
			}
		}
	}*/

}

void AHG_EnterTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor->IsA<AGW_Player>()) {
	//	EnterTriggered();
	//}
}

