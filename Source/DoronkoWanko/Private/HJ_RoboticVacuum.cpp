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

	//충돌체 생성
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp"));
	CapsuleComp->SetupAttachment(RootComponent);

	// 전원버튼 생성(Green)
	ColorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorComp"));
	ColorComp->SetupAttachment(RootComponent);

	// 전원버튼 생성(Red)
	ColorComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColorComp2"));
	ColorComp2->SetupAttachment(RootComponent);

	// 회전 기본값 지정 
	RotationSpeed = 20.0f;
	CurrentRotationAngle = 0.0f;
	Check = 0;
	// 충돌체 처리
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionProfileName(TEXT("MapObject"));
	// 위젯 문구 생성 
	InteractionText = FText::FromString(TEXT("Interaction"));

	// 페인트 스폰 Arrow 생성 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARoboticVacuum::BeginPlay()
{
	Super::BeginPlay();
	CheckAngle = GetActorRotation();
	// 전원버튼 숨긴 채 시작 
	ColorComp->SetVisibility(false);
	ColorComp2->SetVisibility(false);
}

// Called every frame
void ARoboticVacuum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTurnOn)
	{
		FRotator VacuumRotation = FRotator(0.0f, CurrentRotationAngle, 0.0f);
		BoxComp->SetRelativeRotation(VacuumRotation);
		// 회전이 끝난 뒤 앞으로 전진한다 
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime, true);
	}

	if (SpawnCheck>0)
	{
		// 페인트에 닿았을 때, 페인트 스폰
		CurrTime += DeltaTime;

		if (CurrTime > MakeTime)
		{
			FTransform T = InkArrow->GetComponentTransform();
			auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);
			if (Ink) {
				Ink->MeshComp->SetVisibility(false);
				Ink->Initalize(FVector(0, -50, 0));
				Ink->SetMyColor(MyColor);
				Ink->bSpawnedByRV = true;
			}
			CurrTime = 0;
		}
		SpawnTimeCheck+= DeltaTime;
		// 10초 이상 지나면 정지 
		if (SpawnTimeCheck > 10)
		{
			SpawnCheck = 0;	Speed = 0;
			ColorComp->SetVisibility(false);
			ColorComp2->SetVisibility(false);
			StopCheck++;
		}
		// 정지 후에 초기화 
		if (StopCheck > 0)
		{ SpawnCheck = 0;	SpawnTimeCheck = 0;	bTurnOn = false;	}
	}
}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bTurnOn)
	{
		if (OtherActor)
		{
			if (Check < 10)
			{
				if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
				{
					Speed = 0;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.03f, true);
				}
			}

			if (OtherActor->IsA<AHG_Splatter>())
			{
				AHG_Splatter* Splatter = Cast<AHG_Splatter>(OtherActor);
				MyColor = Splatter->MyColor;
				SpawnCheck++;	StopCheck = 0;
				ColorComp2->SetVisibility(true);
			}
		}
	}
}

void ARoboticVacuum::Rotate()
{
	CurrentRotationAngle += RotationSpeed * 1;
	Check += 1;
	// 회전할 때 충돌체 끄고 
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
	UGameplayStatics::PlaySound2D(GetWorld(), VacuumSFX);

	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARoboticVacuum::Rotate, 0.03f, true);
	}
}

void ARoboticVacuum::TurnOn()
{
	// 앞으로 갈때 충돌체 켠다 
	CapsuleComp->SetCollisionProfileName(TEXT("MapObject"));
}


