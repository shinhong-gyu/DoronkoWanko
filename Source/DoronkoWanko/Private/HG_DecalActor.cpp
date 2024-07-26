// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AHG_DecalActor::AHG_DecalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(Decal);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(RootComponent);

	// ·£´ýÇÏ°Ô Decal ÆÐÅÏ Âï¾î³»±â
	UMaterial* SelectedMaterial = nullptr;
	int32 randInt = FMath::RandRange(0,9);
	if (randInt == 1) {
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT(""));
		if (tempMaterial.Succeeded())
		{
			SelectedMaterial = tempMaterial.Object;
		}
	}
	else{
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT(""));
		if (tempMaterial.Succeeded())
		{
			SelectedMaterial = tempMaterial.Object;
		}
	}
	if (SelectedMaterial)
	{
		Decal->SetMaterial(0, SelectedMaterial);
	}
}

// Called when the game starts or when spawned
void AHG_DecalActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHG_DecalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}