// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Paintballoon.h"
#include "HG_Splatter.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GW_Player.h"

// Sets default values
AHG_Paintballoon::AHG_Paintballoon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AHG_Paintballoon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHG_Paintballoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHG_Paintballoon::InteractionWith()
{
	FVector InitialVelocity;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if(Widget != nullptr) Widget->RemoveFromParent();
	Destroy();
	for (int i = 0; i < 5; i++) {
		InitialVelocity = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(300, 600));
		auto* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, SpawnLocation, SpawnRotation);
		if (Splatter) {
			Splatter->Initalize(InitialVelocity);
		}
		auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player) {
			Player->LookAtActor = nullptr;
		}
	}
}


