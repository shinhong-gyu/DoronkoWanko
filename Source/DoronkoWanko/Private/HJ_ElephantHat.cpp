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

	// ������ġ(Arrow) ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(90, 0, 0));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// �浹ü ó�� 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));

	// ���� ���� 
	InteractionText = FText::FromString(TEXT("E) Put On"));
}

// Called when the game starts or when spawned
void AHJ_ElephantHat::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ��Ʈ 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
}

// Called every frame
void AHJ_ElephantHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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

void AHJ_ElephantHat::SpawnInk()
{
	// ���� �����ϱ� (�� �������� Ƣ�����)
	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);
	if (nullptr != Ink)
	{
		Ink->Initalize(FVector(0, 0, 300) + GW_Player->GetActorForwardVector() * 600);
	}
}

