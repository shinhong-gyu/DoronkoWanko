// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterTrigger.h"
#include "ObjectWidget.h"
#include "HG_EnterInstruction.h"
#include "Components/BoxComponent.h"
#include "GW_Player.h"
#include "HJMiniMapWidget.h"

// Sets default values
AHG_EnterTrigger::AHG_EnterTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트 생성
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));

	// 루트 컴포넌트로 설정
	SetRootComponent(BoxComp);

	//BoxComp->SetGenerateOverlapEvents(true);
	//BoxComp->SetCollisionProfileName(TEXT("EnterTrigger"));

	// 방 이름 초기화
	RoomName = FText::FromString(TEXT("Default"));
}

// Called when the game starts or when spawned
void AHG_EnterTrigger::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 함수 바인드
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_EnterTrigger::OnMyBeginOverlap);
}

void AHG_EnterTrigger::EnterTriggered()
{
	// 플레이어를 가져와서 
	auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// 플레이어가 nullptr이 아닐 때
	if (Player) 
	{
		// 플레이어의 현재 방 상태를 확인하
		if (CurrentRoomState != Player->PlayerRoomState) 
		{
			if (Player->EnterWidget == nullptr)
			{
				Player->SetLocState(CurrentRoomState);
				Player->EnterWidget = CreateWidget<UHG_EnterInstruction>(GetWorld(), Player->WidgetFactory);
				if (Player->EnterWidget) 
				{
					Player->EnterWidget->AddToViewport();
					Player->EnterWidget->SetText(RoomName);
				}
			}
			else
			{
				Player->EnterWidget->RemoveFromParent();
				Player->EnterWidget = nullptr;
				EnterTriggered();
			}
		}
	}
	switch (CurrentRoomState)
	{
	case EPlayerRoomState::KITCHEN:
		Player->MinimapUI->ShowFloor(1);
		break;
	case EPlayerRoomState::LIVINGROOM:
		Player->MinimapUI->ShowFloor(1);
		break;
	case EPlayerRoomState::BASEMENTLIVINGROOM:
		Player->MinimapUI->ShowFloor(0);
		break;
	case EPlayerRoomState::WINECELLAR:
		Player->MinimapUI->ShowFloor(0);
		break;
	case EPlayerRoomState::NURSERY:
		Player->MinimapUI->ShowFloor(2);
		break;
	}
}

void AHG_EnterTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGW_Player>()) 
	{
		EnterTriggered();
	}
}

