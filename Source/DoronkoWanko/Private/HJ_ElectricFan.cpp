// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "GW_Player.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// �浹ü ���� 
	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//SetRootComponent(BoxComp);
	//BoxComp->SetBoxExtent(FVector(50));
	//// �޽� ���� 
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(RootComponent);

	// �浹ü ó�� 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapEnd);

	// ���� �޽� ���� 
	WingComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingComp"));
	WingComp->SetupAttachment(RootComponent);
	// ȸ�� �⺻�� ���� 
	RotationSpeed = 200.0f;
	CurrentRotationAngel = 0.0f;

	IsInRange = false;

	InteractionText = FText::FromString(TEXT("E) INTERACTION"));
}

// Called when the game starts or when spawned
void AHJ_ElectricFan::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHJ_ElectricFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �ȿ� ������ �� OnAction �Լ��� ȣ�� 
	if (IsInRange)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
		{
			RotationSpeed = 100.0f;
			PressE = PressE + 1;
		}
	}
	
	if (PressE % 2 == 1)
	{
		// ��ǳ�� ���� ���ư��� �Ѵ�
		// EŰ�� �ѹ� ������ ���ư��� & �ι� ������ ���ư��°� ����� 
		CurrentRotationAngel += RotationSpeed * DeltaTime;
		if (CurrentRotationAngel > 360.0f)
		{
			CurrentRotationAngel -= 360.0f;
		}

		FRotator WingRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
		WingComp->SetRelativeRotation(WingRotation);

		//�浹ü�� ��ȯ�Ѵ� (������ �ε����� Ư�� ��ġ�� ������ ����������) 
		// else �� ��, �浹ü�� �ı��Ѵ� (���� ����) 
	}

}
