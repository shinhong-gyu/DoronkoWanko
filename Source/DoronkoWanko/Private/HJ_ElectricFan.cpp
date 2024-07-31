// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InputComp = nullptr;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	UE_LOG(LogTemp, Warning, TEXT("E key Presse"));
}

// Called when the game starts or when spawned
void AHJ_ElectricFan::BeginPlay()
{
	Super::BeginPlay();
	

	// Ư�������� �����Ǹ� �����Ѵ� (�ٸ� ��ü���� ����) 
}

// Called every frame
void AHJ_ElectricFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// EŰ�� ������ ������ �����Ѵ�
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			EnableInput(PlayerController);
			if (!InputComp)
			{
				InputComp = NewObject<UInputComponent>(this);
				InputComp->RegisterComponent();
			}

			InputComp->BindAction("Interact", IE_Pressed, this, &AHJ_ElectricFan::Interact);
		}
		 
	// �÷��̾ ������ ���� �ν��Ѵ� (�����Լ�) 
	// ������ 2������ ������. A�� B 
}

void AHJ_ElectricFan::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("E key Presse"));
}
// A�� ��, ��ǳ�� ���� ���ư���. 
// A�� ��, ������ �浹ü ��ȯ�Ѵ� (��ǳ�� �ٶ�����) // �浹ü�� ���� ���� �̵��� ���� ��ü���� ���� 

// B�� ��, ��ǳ�� ���� �����. 
// B�� ��, ������ �浹ü �������. 
