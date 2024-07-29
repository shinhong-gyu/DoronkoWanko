// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ARoboticVacuum::ARoboticVacuum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 생성 
	/*SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));*/
	//SetRootComponent(SphereComp);

	// 회전 기본값 지정 
	RotationSpeed = 20.0f;
	CurrentRotationAngle = 0.0f;

	Check = 0;

	InteractionText = FText::FromString(TEXT("E) PRESS"));

}

// Called when the game starts or when spawned
void ARoboticVacuum::BeginPlay()
{
	Super::BeginPlay();
	CheckAngle = GetActorRotation();
	
}

// Called every frame
void ARoboticVacuum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator VacuumRotation = FRotator(0.0f, CurrentRotationAngle, 0.0f);
	BoxComp->SetRelativeRotation(VacuumRotation);

	if (MoveCheck >= 20)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector()*Speed*DeltaTime);
	}
}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor)
	{
		MoveCheck = 0;
		if (Check < 20)
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
			{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.05f, true);
			}
		}

		else
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}

	}
}

void ARoboticVacuum::Rotate()
{
	CurrentRotationAngle += RotationSpeed * 1;
	Check += 1;
	MoveCheck += 1;

	if (Check >= 20)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Check = 0; 
	}
}


