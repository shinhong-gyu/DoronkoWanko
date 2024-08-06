// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_FanWingSplatter.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "HJ_FanWing.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
AHJ_FanWingSplatter::AHJ_FanWingSplatter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionProfileName(TEXT("FanWing"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHJ_FanWingSplatter::BeginPlay()
{
	Super::BeginPlay();
	// ��ǳ�� ���� ĳ��Ʈ 
	HJ_Fan = Cast<AHJ_FanWing>(UGameplayStatics::GetActorOfClass(GetWorld(), HJ_FanWing));
	// �÷��̾� ĳ��Ʈ 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

}

// Called every frame
void AHJ_FanWingSplatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHJ_FanWingSplatter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<AHG_Splatter>())
	{
		UE_LOG(LogTemp, Warning, TEXT("HitSplatter"));
		OtherActor->Destroy();  
		// ���� �����ϱ� (��ǳ�� �ٶ� �������� Ƣ�����) 
		FTransform T = InkArrow->GetComponentTransform();
		auto* Ink = GetWorld()->SpawnActor<AHJ_Splatter2>(InkFactory, T);
		FVector RandomInitial = FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), FMath::RandRange(400, 800));

		if (nullptr != Ink)
		{
			Ink->Initalize(RandomInitial + FVector(HJ_Fan->GetActorRightVector() * 1000));
		}
	}
}

