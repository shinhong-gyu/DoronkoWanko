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

	// �浹ü ���� 
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50));
	// �޽� ���� 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	// ������ġ(Arrow) ���� 
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

	// �÷��̾� EŰ �Է� �ޱ� 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
	{
		PressE += 1;
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

void AHJ_ElephantHat::SpawnInk()
{
	// ���� �����ϱ� (�� �������� Ƣ�����)
	FTransform T = InkArrow->GetComponentTransform();
	GetWorld()->SpawnActor<AHJTestInk>(InkFactory, T);
}

