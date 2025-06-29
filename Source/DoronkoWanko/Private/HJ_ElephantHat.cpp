// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElephantHat.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GW_Player.h"
#include "HG_Splatter.h"

// Sets default values
AHJ_ElephantHat::AHJ_ElephantHat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스폰위치(Arrow) 생성 
	SplatterArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	SplatterArrow->SetupAttachment(RootComponent);
	SplatterArrow->SetRelativeLocation(FVector(90, 0, 0));
	SplatterArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));

	// 위젯 생성 
	InteractionText = FText::FromString(TEXT("Put On"));

	MyColor = FLinearColor(1.0f, 0.5f, 0.946f, 1.0f);
}

// Called when the game starts or when spawned
void AHJ_ElephantHat::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스트 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

}

// Called every frame
void AHJ_ElephantHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTurnOn)
	{
		if (GW_Player)
		{
			CurrTime += DeltaTime;

			if (CurrTime > MakeTime)
			{
				SpawnSplatter();
				CurrTime = 0;
			}
		}
	}
}

void AHJ_ElephantHat::SpawnSplatter()
{
	// 물감 스폰하기 (코 앞쪽으로 튀어나가게)
	FTransform T = SplatterArrow->GetComponentTransform();
	auto* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, T);
	if (nullptr != Splatter)
	{
		Splatter->Initalize(FVector(0, 0, 600) + GW_Player->GetActorForwardVector() * 600);
		Splatter->SetMyColor(MyColor);
	}
}

void AHJ_ElephantHat::InteractionWith()
{
	// 플레이어에서 인터렉션 켜줄 때
	bTurnOn = true;
}

void AHJ_ElephantHat::ItemDrop()
{
	bTurnOn = false;
	// 장착 해제 후 땅에 두기
	SetActorLocation(GW_Player->GetActorLocation() + FVector(50, 0, -80));
}

