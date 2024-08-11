// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_TrainWall.h"
#include "Components/BoxComponent.h"
#include "HJ_Train.h"

// Sets default values
AHJ_TrainWall::AHJ_TrainWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHJ_TrainWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_TrainWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHJ_TrainWall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<AHJ_Train>())
	{
		this->Destroy();
	}
}



