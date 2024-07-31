// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicObject.h"
#include "GW_Player.h"

ADynamicObject::ADynamicObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADynamicObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADynamicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADynamicObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어와 충돌 인식 
	auto* Player = Cast<AGW_Player>(OtherActor);

	if (Player)
	{
		IsInRange = true;
		UE_LOG(LogTemp, Warning, TEXT("CHECK"));
	}

}

void ADynamicObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어와 충돌 종료 인식 
	auto* Player = Cast<AGW_Player>(OtherActor);

	if (Player)
	{
		IsInRange = false;
		UE_LOG(LogTemp, Warning, TEXT("CHECKOUT"));
	}
}