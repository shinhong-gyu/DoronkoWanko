// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Cake.h"
#include "Kismet/GameplayStatics.h"
#include "HJ_EndingWidget.h"

// Sets default values
AHJ_Cake::AHJ_Cake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 困连 巩备 积己 
	InteractionText = FText::FromString(TEXT("Ending"));

}

// Called when the game starts or when spawned
void AHJ_Cake::BeginPlay()
{
	Super::BeginPlay();
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
}

// Called every frame
void AHJ_Cake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHJ_Cake::InteractionWith()
{
	EndingUI = Cast<UHJ_EndingWidget>(CreateWidget(GetWorld(), EndingUIFactory));
	EndingUI->AddToViewport();
}

