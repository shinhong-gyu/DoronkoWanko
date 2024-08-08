// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Train.h"
#include "Components/BoxComponent.h"
#include "HJ_TrainWheel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "GW_Player.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHJ_Train::AHJ_Train()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 
	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//SetRootComponent(BoxComp);
	//BoxComp->SetBoxExtent(FVector(40.0f, 60.0f, 40.0f));
	// 메쉬 생성
	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);*/

	// 잉크 스폰 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(BoxComp);

	// 열차 바퀴 메쉬 생성
	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel1->SetupAttachment(MeshComp);
	Wheel2->SetupAttachment(MeshComp);

	// 바퀴 안보이게 설정 
	Wheel1->SetVisibility(false);
	Wheel2->SetVisibility(false);

	// 열차 회전 변수 
	Radius = 500.0f;
	AngularSpeed = 2.0f;
	CurrentAngle = 0.0f;

	// 회전 기본값 지정 
	RotationSpeed = 115.0f;
	CurrentRotationAngel = 0.0f;
	// 위젯 문구 생성 
	InteractionText = FText::FromString(TEXT("E) Interaction"));
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스트 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AHJ_Train::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WheelCheck == 5)
	{
		Wheel1->SetVisibility(true);
	}

	if (WheelCheck == 10)
	{
		Wheel2->SetVisibility(true);
	}

	if (WheelCheck > 10)
	{
		CurrentAngle += AngularSpeed * DeltaTime;
		float X = Radius * FMath::Cos(CurrentAngle);
		float Y = Radius * FMath::Sin(CurrentAngle);
		float Z = GetActorLocation().Z;

		SetActorLocation(FVector(X - 2100.f, Y - 3450.f, Z)); // 위치 정해지면 FVector(X,Y,Z) 더해주기 

		CurrentRotationAngel += RotationSpeed * DeltaTime;
		if (CurrentRotationAngel > 360.0f)
		{
			CurrentRotationAngel -= 360.0f;
		}
		FRotator TrainRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
		BoxComp->SetRelativeRotation(TrainRotation);

		CurrTime += DeltaTime;

		if (CurrTime > MakeTime)
		{
			// 물감 스폰하기 (정수리에서 Ink 가 Spawn 되도록)

			FTransform T = InkArrow->GetComponentTransform();
			auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

			if (nullptr != Ink) {
				Ink->Initalize(FVector(-100, 0, 0));
			}
			CurrTime = 0;
		}
	}
}

void AHJ_Train::InteractionWith()
{
	WheelCheck += 1;
}

void AHJ_Train::ItemDrop()
{
	bTurnOn = false;
}



