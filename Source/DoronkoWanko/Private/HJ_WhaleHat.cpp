// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_WhaleHat.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GW_Player.h"
#include "HG_Splatter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_WhaleHat::AHJ_WhaleHat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������ġ(Arrow) ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(0, 0, 100));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// �浹ü ó�� 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));

	// ���� ���� 
	InteractionText = FText::FromString(TEXT("Put On"));
}

// Called when the game starts or when spawned
void AHJ_WhaleHat::BeginPlay()
{
	Super::BeginPlay();
	// �÷��̾� ĳ��Ʈ 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
}

// Called every frame
void AHJ_WhaleHat::Tick(float DeltaTime)
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

void AHJ_WhaleHat::SpawnInk()
{
	// ���� �����ϱ� (���������� Ink �� Spawn �ǵ���)

	FVector InitialVelocity = FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), FMath::RandRange(800, 1000));

	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

	if (nullptr != Ink) {
		Ink->Initalize(InitialVelocity);
	}
}

void AHJ_WhaleHat::InteractionWith()
{
	bTurnOn = true;
}

void AHJ_WhaleHat::ItemDrop()
{
	bTurnOn = false;
	// ���� ���� �� ���� �α�
	SetActorLocation(GW_Player->GetActorLocation() + FVector(50, 0, -80));
}

