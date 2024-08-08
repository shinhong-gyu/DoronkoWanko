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

	// �浹ü ���� 
	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//SetRootComponent(BoxComp);
	//BoxComp->SetBoxExtent(FVector(40.0f, 60.0f, 40.0f));
	// �޽� ����
	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);*/

	// ��ũ ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(BoxComp);

	// ���� ���� �޽� ����
	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel1->SetupAttachment(MeshComp);
	Wheel2->SetupAttachment(MeshComp);

	// ���� �Ⱥ��̰� ���� 
	Wheel1->SetVisibility(false);
	Wheel2->SetVisibility(false);

	// ���� ȸ�� ���� 
	Radius = 500.0f;
	AngularSpeed = 2.0f;
	CurrentAngle = 0.0f;

	// ȸ�� �⺻�� ���� 
	RotationSpeed = 115.0f;
	CurrentRotationAngel = 0.0f;
	// ���� ���� ���� 
	InteractionText = FText::FromString(TEXT("E) Interaction"));
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ��Ʈ 
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

		SetActorLocation(FVector(X - 2100.f, Y - 3450.f, Z)); // ��ġ �������� FVector(X,Y,Z) �����ֱ� 

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
			// ���� �����ϱ� (���������� Ink �� Spawn �ǵ���)

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



