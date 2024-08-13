// Fll out your copyright notice in the Description page of Project Settings.


#include "HG_MissonStamp.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"

// Sets default values
AHG_MissonStamp::AHG_MissonStamp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(Decal);
	int RandNum = FMath::RandRange(0, 1);
	if (RandNum == 0) {
		static ConstructorHelpers::FObjectFinder<UMaterial> FindMaterial1(TEXT("/Script/Engine.Material'/Game/HongGyu/M_MissonStamp1.M_MissonStamp1''"));
		if (FindMaterial1.Succeeded()) {
			DecalMaterial = FindMaterial1.Object;
		}
	}
	else {
		static ConstructorHelpers::FObjectFinder<UMaterial> FindMaterial1(TEXT("/Script/Engine.Material'/Game/HongGyu/M_MissonStamp1.M_MissonStamp2''"));
		if (FindMaterial1.Succeeded()) {
			DecalMaterial = FindMaterial1.Object;
		}
	}
	Decal->SetMaterial(0, DecalMaterial);
	Decal->DecalSize = FVector(32.0f, 32.0f, 32.0f);
	Decal->SetSortOrder(1);
}

// Called when the game starts or when spawned
void AHG_MissonStamp::BeginPlay()
{
	Super::BeginPlay();

	Decal->SetVisibility(false);
}

// Called every frame
void AHG_MissonStamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

