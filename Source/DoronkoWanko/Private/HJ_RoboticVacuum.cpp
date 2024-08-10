// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GW_Player.h"
#include "Delegates/Delegate.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ARoboticVacuum::ARoboticVacuum()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�浹ü ����
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp"));
	CapsuleComp->SetupAttachment(RootComponent);

	// ������ư ����
	ColorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorComp"));
	ColorComp->SetupAttachment(RootComponent);

	// ȸ�� �⺻�� ���� 
	RotationSpeed = 20.0f;
	CurrentRotationAngle = 0.0f;
	Check = 0;
	// �浹ü ó��
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionProfileName(TEXT("MapObject"));
	// ���� ���� ���� 
	InteractionText = FText::FromString(TEXT("E) Interaction"));

	// ����Ʈ ���� Arrow ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARoboticVacuum::BeginPlay()
{
	Super::BeginPlay();
	CheckAngle = GetActorRotation();
	// ������ư ���� ä ���� 
	ColorComp->SetVisibility(false);

}

// Called every frame
void ARoboticVacuum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTurnOn)
	{
		FRotator VacuumRotation = FRotator(0.0f, CurrentRotationAngle, 0.0f);
		BoxComp->SetRelativeRotation(VacuumRotation);
		// ȸ���� ���� �� ������ �����Ѵ� 
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime, true);
	}

	if (SpawnCheck > 0)
	{
		// ����Ʈ�� ����� ��, ����Ʈ ����
		CurrTime += DeltaTime;

		if (CurrTime > MakeTime)
		{
			// ���� �����ϱ� (���������� Ink �� Spawn �ǵ���)

			FTransform T = InkArrow->GetComponentTransform();
			auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

			if (nullptr != Ink) {
				Ink->Initalize(FVector(-500, 0, 0));
			}
			CurrTime = 0;
		}
	}
}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bTurnOn)
	{
		if (OtherActor)
		{
			MoveCheck = 0;
			if (Check < 10)
			{
				if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
				{
					Speed = 0;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.03f, true);
				}
			}
		}

		if (OtherActor->IsA<AHG_Splatter>())
		{
			SpawnCheck ++;
		}
	}
}

void ARoboticVacuum::Rotate()
{
	CurrentRotationAngle += RotationSpeed * 1;
	Check += 1;
	MoveCheck += 1;
	// ȸ���� �� �浹ü ���� 
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Check >= 10)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Check = 0;
		Speed = 200;
	}
}

void ARoboticVacuum::InteractionWith()
{
	bTurnOn = true; 
	ColorComp->SetVisibility(true);

	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.03f, true);
	}
}

void ARoboticVacuum::TurnOn()
{
	// ������ ���� �浹ü �Ҵ� 
	CapsuleComp->SetCollisionProfileName(TEXT("MapObject"));
}


