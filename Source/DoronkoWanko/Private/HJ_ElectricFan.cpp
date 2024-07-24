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

	// 플레이어가 가까이 오면 인식한다 (공통함수) 
	// 조건을 2가지로 나눈다. A와 B 
	// 
// A일 때, 선풍기 날개 돌아간다. 
// A일 때, 투명한 충돌체 소환한다 (선풍기 바람범위) // 충돌체에 따른 물감 이동은 물감 객체에서 구현 

// B일 때, 선풍기 날개 멈춘다. 
// B일 때, 투명한 충돌체 사라진다. 
