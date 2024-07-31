// Fill out your copyright notice in the Description page of Project Settings.


#include "helmet.h"

// Sets default values
Ahelmet::Ahelmet()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Ahelmet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Ahelmet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Ahelmet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

