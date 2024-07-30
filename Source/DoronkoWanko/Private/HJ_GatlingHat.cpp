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

	// ������ġ(Arrow) ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(120, 0, 0));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// �浹ü ó�� 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_GatlingHat::OnOverlapBegin); //���� �� �̸� 
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_GatlingHat::OnOverlapEnd);

	// ���� ���� 
	InteractionText = FText::FromString(TEXT("E) PRESS"));

}

// Called when the game starts or when spawned
void AHJ_GatlingHat::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHJ_GatlingHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInRange)
	{
		// �÷��̾� EŰ �Է� �ޱ� 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->WasInputKeyJustPressed(EKeys::E))
		{
			PressE += 1;
			UE_LOG(LogTemp, Warning, TEXT("PressE"));
		}
	}
	// EŰ�� �Է����� �� 
	if (PressE >= 1)
	{
		CurrTime += DeltaTime;

		if (CurrTime > MakeTime)
		{
			SpawnInk();
			CurrTime = 0;
		}

	}

}

void AHJ_GatlingHat::SpawnInk()
{
	// ���� �����ϱ� (���� �������� Ƣ�����) 
	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);
	if (nullptr != Ink)
	{
		Ink->Initalize(FVector(1500, 0, 500));
	}
}

