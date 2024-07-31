// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_TrainWheel.h"
#include "GW_Player.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_TrainWheel::AHJ_TrainWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// MasterInteraction에서 정의 
	/*BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(15.0f, 15.0f, 5.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);*/

	InteractionText = FText::FromString(TEXT("E) PRESS"));

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

}

