// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_GatlingHat.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HG_Splatter.h"
#include "GW_Player.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_GatlingHat::AHJ_GatlingHat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스폰위치(Arrow) 생성 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(120, 0, 0));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_GatlingHat::OnOverlapBegin); //변경 전 이름 
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_GatlingHat::OnOverlapEnd);

	// 위젯 생성 
	InteractionText = FText::FromString(TEXT("E) PRESS"));

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

	if (IsInRange)
	{
		if (GW_Player)
		{

		}
		bItemRange = true;
	}

	if (GW_Player&&bItemRange)
	{
		/*if (GW_Player->bFire)
		{
			CurrTime += DeltaTime;

			if (CurrTime > MakeTime)
			{
				SpawnInk();
				CurrTime = 0;
			}
		}*/
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

