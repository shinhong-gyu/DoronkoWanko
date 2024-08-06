// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_CoffeeCup.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_CoffeeCup::AHJ_CoffeeCup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 설정
	MeshComp->OnComponentHit.AddDynamic(this, &AHJ_CoffeeCup::OnHit);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// 위젯 문구 생성 
	InteractionText = FText::FromString(TEXT("E) INTERACTION"));

}

// Called when the game starts or when spawned
void AHJ_CoffeeCup::BeginPlay()
{
	Super::BeginPlay();
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AHJ_CoffeeCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHJ_CoffeeCup::SpawnInk()
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, Location, Rotation);
	if (Ink)
	{
		FVector InitialVelocity = FVector(FMath::RandRange(-10, 10), FMath::RandRange(-10, 10), FMath::RandRange(200, 400));
		Ink->Initalize(InitialVelocity);
	}
}

void AHJ_CoffeeCup::InteractionWith()
{
}

void AHJ_CoffeeCup::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<AGW_Player>())
	{
		UE_LOG(LogTemp, Warning, TEXT("CoffeeCalled"));
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHJ_CoffeeCup::SpawnInk, 1.0f, true);
		}
	}
}


