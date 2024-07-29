// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElephantHat.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_ElephantHat::AHJ_ElephantHat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50));
	// 메쉬 생성 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	// 스폰위치(Arrow) 생성 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
	InkArrow->SetRelativeLocation(FVector(90, 0, 0));
	InkArrow->SetRelativeRotation(FRotator(0, 0, 0));
}

// Called when the game starts or when spawned
void AHJ_ElephantHat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_ElephantHat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 E키 입력 받기 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
	{
		PressE += 1;
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

void AHJ_ElephantHat::SpawnInk()
{
	// 물감 스폰하기 (코 앞쪽으로 튀어나가게)
	FTransform T = InkArrow->GetComponentTransform();
	GetWorld()->SpawnActor<AHJTestInk>(InkFactory, T);
}

