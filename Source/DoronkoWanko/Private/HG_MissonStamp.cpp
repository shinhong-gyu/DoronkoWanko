// Fill out your copyright notice in the Description page of Project Settings.


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

	static ConstructorHelpers::FObjectFinder<UMaterial> FindMaterial(TEXT("/Script/Engine.Material'/Game/HongGyu/M_MissonStampDecal.M_MissonStampDecal''"));
	if (FindMaterial.Succeeded()) {
		DecalMaterial = FindMaterial.Object;
		Decal->SetMaterial(0, DecalMaterial);
		Decal->DecalSize = FVector(32.0f,32.0f,32.0f);
		Decal->SetSortOrder(1);
	}
}

// Called when the game starts or when spawned
void AHG_MissonStamp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHG_MissonStamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

