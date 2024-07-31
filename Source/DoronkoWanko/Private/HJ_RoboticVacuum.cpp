// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GW_Player.h"
#include "Delegates/Delegate.h"
#include "Components/CapsuleComponent.h"

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
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ARoboticVacuum::OnOverlapBegin); //���� �� �̸� 
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ARoboticVacuum::OnOverlapEnd);
	// ���� ���� ���� 
	InteractionText = FText::FromString(TEXT("E) PRESS"));

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
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
	}
}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bTurnOn)
	{
		if (OtherActor)
		{
			MoveCheck = 0;
			if (Check < 20)
			{
				if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
				{
					Speed = 0;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.03f, true);
				}
			}
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

	if (Check >= 20)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Check = 0;
		Speed = 300;
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




