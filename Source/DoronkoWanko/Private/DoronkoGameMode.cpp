// Fill out your copyright notice in the Description page of Project Settings.


#include "DoronkoGameMode.h"
#include "HG_ScoreBoard.h"
#include "HJ_ElectricFan.h"

void ADoronkoGameMode::BeginPlay()
{
	Super::BeginPlay();

	ScoreBoard = Cast<UHG_ScoreBoard>(CreateWidget(GetWorld(), ScoreBoardFactory, FName("ScoreBoard")));
	if (nullptr != ScoreBoard) {
		ScoreBoard->AddToViewport();
	}
}

void ADoronkoGameMode::SetScore(int32 Point)
{
	GameScore += Point;

	if (GameScore > 1200)
	{
		SpawnWingFan();
	}
}

void ADoronkoGameMode::UpdataScoreBoard()
{
	ScoreBoard->SetText(GameScore);
}

void ADoronkoGameMode::SpawnWingFan()
{	
	FanTransform.SetLocation(FVector(-936, 623, 319));
	FanTransform.SetRotation(FQuat(FRotator(0, 180, 0)));
	FanTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_ElectricFan>(ElectricFanFactory, FanTransform);
}
