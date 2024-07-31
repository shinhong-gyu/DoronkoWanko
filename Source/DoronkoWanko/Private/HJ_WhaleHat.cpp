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

	// 스폰위치(Arrow) 생성 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(0, 0, 100));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_WhaleHat::OnOverlapBegin); //변경 전 이름 
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_WhaleHat::OnOverlapEnd);

	// 위젯 생성 
	InteractionText = FText::FromString(TEXT("E) PRESS"));
}

// Called when the game starts or when spawned
void AHJ_WhaleHat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_WhaleHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInRange)
	{
		// 플레이어 E키 입력 받기 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->WasInputKeyJustPressed(EKeys::E))
		{
			PressE += 1;
			UE_LOG(LogTemp, Warning, TEXT("PressE"));
		}
	}
	// E키를 입력했을 때 
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

void AHJ_WhaleHat::SpawnInk()
{
	// 물감 스폰하기 (코 앞쪽으로 튀어나가게)

	FVector InitialVelocity = FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), FMath::RandRange(600, 800));

	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

	if (nullptr != Ink) {
		Ink->Initalize(InitialVelocity);
	}
}

