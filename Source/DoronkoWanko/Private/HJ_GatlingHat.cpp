// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_GatlingHat.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GW_Player.h"
#include "HG_Splatter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_GatlingHat::AHJ_GatlingHat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스폰위치(Arrow) 생성 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(0, 0, 100));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));

	// 위젯 생성 
	InteractionText = FText::FromString(TEXT("E) PUT ON"));

}

// Called when the game starts or when spawned
void AHJ_GatlingHat::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스트 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

}

// Called every frame
void AHJ_GatlingHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTurnOn)
	{
		if (GW_Player)
		{
			CurrTime += DeltaTime;

			if (CurrTime > MakeTime)
			{
				SpawnInk();
				CurrTime = 0;
			}
		}
	}
}

void AHJ_GatlingHat::SpawnInk()
{
	// 물감 스폰하기 (대포 앞쪽으로 튀어나가게) 
	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);
	if (nullptr != Ink)
	{
		Ink->Initalize(FVector(0, 0, 500) + GW_Player->GetActorForwardVector() * 2500);
	}
}

void AHJ_GatlingHat::InteractionWith()
{
	bTurnOn = true;
}

void AHJ_GatlingHat::ItemDrop()
{
	bTurnOn = false;
	// 장착 해제 후 땅에 두기
	SetActorLocation(GW_Player->GetActorLocation() + FVector(50, 0, -110));
}

