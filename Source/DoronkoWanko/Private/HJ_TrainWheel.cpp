// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_TrainWheel.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJ_TrainWheel::AHJ_TrainWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(15.0f, 15.0f, 5.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

}

// Called when the game starts or when spawned
void AHJ_TrainWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_TrainWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Player 다가왔을 때, 인식하는 건 인터페이스 구현 예정 
}

