// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GW_Player.h"
#include "Delegates/Delegate.h"

// Sets default values
ARoboticVacuum::ARoboticVacuum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 생성
	/*SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);*/

	// 회전 기본값 지정 
	RotationSpeed = 20.0f;
	CurrentRotationAngle = 0.0f;
	Check = 0;
	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoboticVacuum::OnOverlapBegin); //변경 전 이름 
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ARoboticVacuum::OnOverlapEnd);
	// 위젯 문구 생성 
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

	if (IsInRange)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
			{	PressE += 2;	}
	}

	if (PressE >= 1)
			{
				FRotator VacuumRotation = FRotator(0.0f, CurrentRotationAngle, 0.0f);
				BoxComp->SetRelativeRotation(VacuumRotation);
				// 회전이 끝난 뒤 앞으로 전진한다 
				if (MoveCheck >= 20)
				{
					SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
				}
			}
}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (PressE >= 1)
	{
		if (OtherActor)
		{
			MoveCheck = 0;
			if (Check < 20)
			{
				if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
				{
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

	if (Check >= 20)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Check = 0;
	}
}




