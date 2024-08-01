// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_TrainBody.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
AHJ_TrainBody::AHJ_TrainBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	Torque = FVector(0.f, 0.f, 100000.f);
}

// Called when the game starts or when spawned
void AHJ_TrainBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_TrainBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MeshComp->IsSimulatingPhysics())
	{
		MeshComp->AddTorqueInDegrees(Torque);
	}
}

