// Fill out your copyright notice in the Description page of Project Settings.


#include "DoronkoGameMode.h"
#include "HG_ScoreBoard.h"
#include "HJ_ElectricFan.h"
#include "Kismet/GameplayStatics.h"

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
		// Lv.1 ��ǳ�� ��ȯ 
		SpawnWingFan();
	}

	if (GameScore > 2400)
	{
		// Lv.2 �������� ��ȯ 
		SpawnTrainWheel();
	}

	if (GameScore > 3600)
	{
		// Lv.3 ������ ��ȯ 
		SpawnWhaleHat();
	}

	if (GameScore > 4800)
	{
		// Lv.4 ��������(2) ��ȯ 
		SpawnTrainWheel2();
	}

	if (GameScore > 5000)
	{
		// Lv.5 ���ι�ư ��ȯ 
		SpawnWineButton();
	}
}

void ADoronkoGameMode::UpdataScoreBoard()
{
	ScoreBoard->SetText(GameScore);
}

void ADoronkoGameMode::SpawnWingFan()
{	
	// ��ǳ�� ��ȯ 
	Transform1.SetLocation(FVector(-936, 623, 319));
	Transform1.SetRotation(FQuat(FRotator(0, 180, 0)));
	Transform1.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_ElectricFan>(ElectricFanFactory, Transform1);

	// ���� �۾� �Ϸ�Ǹ� �ƾ� �߰� �ʿ� 
}

void ADoronkoGameMode::SpawnTrainWheel()
{
	if (bTrainWheel1)
	{ 
	// �������� ��ȯ
	Transform2.SetLocation(FVector(-1491, -4963, -431));
	Transform2.SetRotation(FQuat(FRotator(0, 0, 0)));
	Transform2.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, Transform2);
	}
	// Player�� �����, �߰� Spawn ���� 
	bTrainWheel1 = false;
}

void ADoronkoGameMode::SpawnWhaleHat()
{
	if (bWhaleHat)
	{
	// ������ ��ȯ
	Transform3.SetLocation(FVector(-2358, -1884, 932));
	Transform3.SetRotation(FQuat(FRotator(0, 0, 0)));
	Transform3.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_WhaleHat>(HatFactory, Transform3);
	}
	// Player�� �����, �߰� Spawn ���� 
	bWhaleHat = false;
}

void ADoronkoGameMode::SpawnTrainWheel2()
{
	if (bTrainWheel2)
	{
	// �������� ��ȯ(2)
	Transform4.SetLocation(FVector(-2508, -2443, 885));
	Transform4.SetRotation(FQuat(FRotator(0, 0, 0)));
	Transform4.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, Transform4);
	}
	// Player�� �����, �߰� Spawn ���� 
	bTrainWheel2 = false;
}

void ADoronkoGameMode::SpawnWineButton()
{
	// ���ι�ư ��ȯ 
	Transform5.SetLocation(FVector(-2532, -4966, -513));
	Transform5.SetRotation(FQuat(FRotator(0, 0, 0)));
	Transform5.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetWorld()->SpawnActor<AWIneButton>(ButtonFactory, Transform5);
}


