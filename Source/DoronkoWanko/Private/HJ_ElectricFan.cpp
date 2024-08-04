// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "HJ_FanWingSplatter.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	/*WingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WingArrow"));
	WingArrow->SetupAttachment(RootComponent);

	InteractionText = FText::FromString(TEXT(""));*/
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

//void AHJ_ElectricFan::InteractionWith()
//{
//	UE_LOG(LogTemp, Warning, TEXT("BodyInteraction"));
//	FTransform T = WingArrow->GetComponentTransform();
//	GetWorld()->SpawnActor<AHJ_FanWingSplatter>(WingSplatter, T);
//}

