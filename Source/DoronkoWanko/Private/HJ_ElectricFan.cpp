// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "GW_Player.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapEnd);

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

}

void AHJ_ElectricFan::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* Player = Cast<AGW_Player>(OtherActor);

	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHECK"));
	}

}

void AHJ_ElectricFan::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* Player = Cast<AGW_Player>(OtherActor);

	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHECKOUT"));
	}
}

	// �÷��̾ ������ ���� �ν��Ѵ� (�����Լ�) 
	// ������ 2������ ������. A�� B 
	// 
// A�� ��, ��ǳ�� ���� ���ư���. 
// A�� ��, ������ �浹ü ��ȯ�Ѵ� (��ǳ�� �ٶ�����) // �浹ü�� ���� ���� �̵��� ���� ��ü���� ���� 

// B�� ��, ��ǳ�� ���� �����. 
// B�� ��, ������ �浹ü �������. 
