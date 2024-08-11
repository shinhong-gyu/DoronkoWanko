// Fill out your copyright notice in the Description page of Project Settings.


#include "WIneButton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "HG_Splatter.h"

// Sets default values
AWIneButton::AWIneButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(RootComponent);

	Red = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Red"));
	Red->SetupAttachment(RootComponent);

	Red->SetVisibility(false);

	// 위젯 문구 생성 
	InteractionText = FText::FromString(TEXT("E) Interaction"));
}

// Called when the game starts or when spawned
void AWIneButton::BeginPlay()
{
	Super::BeginPlay();
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AWIneButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWIneButton::InteractionWith()
{
	Red->SetVisibility(true);
	MeshComp->SetVisibility(true);

	for (int i = 0; i < 500; i++) {
		Shake();
	}

	// 물감 터뜨리기 or 와인거치대 기울여서 떨어뜨리기 
	// 기능 확정된 후 추가 함수 기재하기 
}

void AWIneButton::Shake()
{
	FVector InitialVelocity = FVector(FMath::RandRange(-1200, 1200), FMath::RandRange(-1200, 1200), FMath::RandRange(-1200, 1200));

	FTransform T = InkArrow->GetComponentTransform();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

	if (nullptr != Ink) {
		Ink->MeshComp->SetVisibility(false);
		Ink->Initalize(InitialVelocity);
	}
}

