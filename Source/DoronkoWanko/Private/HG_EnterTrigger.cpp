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

	// 플레이어가 nullptr 이 아닐 때
	if (Player)
	{
		// EnterTrigger가 갖는 방 정보와 플레이어의 현재 위치한 방이 같지 않으면서
		if (CurrentRoomState != Player->PlayerRoomState)
		{
			// 플레이어의 EnterWidget이 생성되지 않은 상태라면
			if (Player->EnterInstructionUI == nullptr)
			{
				// 플레이어의 방 위치 정보를 업데이트
				Player->SetLocState(CurrentRoomState);

				// 플레이어의 EnterWidget을 생성
				Player->EnterInstructionUI = CreateWidget<UHG_EnterInstruction>(GetWorld(), Player->EnterInstructionUIClass);

				// EnterWidget이 생성되었다면
				if (Player->EnterInstructionUI)
				{
					// 화면에 EnterWidget을 팝업시키고
					Player->EnterInstructionUI->AddToViewport();

					// EnterWidget의 방 이름 텍스트를 Trigger가 갖고있는 RoomName으로 설정
					Player->EnterInstructionUI->SetRoomName(RoomName);
				}
			}

			// 만약 이미 EnterWidget이 생성되어있다면
			else
			{
				// 기존의 EnterWidget을 제거하고
				Player->EnterInstructionUI->RemoveFromParent();
				Player->EnterInstructionUI = nullptr;

				// Trigger 로직을 다시 실행
				EnterTriggered();
			}
		}
	}

	// 플레리어의 미니맵이 유효하다면
	if (Player->MinimapUI)
	{
		// 현재 어느 방에 위치해 있는지에 따라서 미니맵 변경
		switch (CurrentRoomState)
		{
		case EPlayerRoomState::KITCHEN:
		case EPlayerRoomState::LIVINGROOM:
			Player->MinimapUI->ShowFloor(1);
			break;
		case EPlayerRoomState::BASEMENTLIVINGROOM:
		case EPlayerRoomState::WINECELLAR:
			Player->MinimapUI->ShowFloor(0);
			break;
		case EPlayerRoomState::NURSERY:
			Player->MinimapUI->ShowFloor(2);
			break;
		}
	}
}

void AHG_EnterTrigger::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 만약 충돌한 액터가 플레이어라면
	if (OtherActor->IsA<AGW_Player>())
	{
		// Trigger 로직 실행
		EnterTriggered();
	}
}

