// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Wine1.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_Wine1::AHJ_Wine1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ����
	MeshComp->OnComponentHit.AddDynamic(this, &AHJ_Wine1::OnHit);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ���� ���� 
	InteractionText = FText::FromString(TEXT("E) INTERACTION"));
	
}

// Called when the game starts or when spawned
void AHJ_Wine1::BeginPlay()
{
	Super::BeginPlay();
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void AHJ_Wine1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHJ_Wine1::SpawnInk()
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, Location, Rotation);
	if (Ink)
	{
		FVector InitialVelocity = FVector(FMath::RandRange(-10, 10), FMath::RandRange(-10, 10), FMath::RandRange(100, 150));
		Ink->Initalize(InitialVelocity);
	}

	/*Check += 4;
	if (Check >= 20)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Check = 0;
	}*/
}

void AHJ_Wine1::InteractionWith()
{
}

void AHJ_Wine1::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<AGW_Player>())
	{
		UE_LOG(LogTemp, Warning, TEXT("WineCalled"));
		if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHJ_Wine1::SpawnInk, 1.0f, true);
		}
	}
}
