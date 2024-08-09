// Fill out your copyright notice in the Description page of Project Settings.


#include "DoronkoGameMode.h"
#include "HG_ScoreBoard.h"
#include "HJ_ElectricFan.h"
#include "Kismet/GameplayStatics.h"
#include "WBP_Doronko_Lv1.h"

void ADoronkoGameMode::BeginPlay()
{
	Super::BeginPlay();

	ScoreBoard = Cast<UHG_ScoreBoard>(CreateWidget(GetWorld(), ScoreBoardFactory, FName("ScoreBoard")));
	if (nullptr != ScoreBoard) {
		ScoreBoard->AddToViewport();
	}

	// 시퀀스 UI를 생성하고 
	Lv1UI = Cast<UWBP_Doronko_Lv1>(CreateWidget(GetWorld(), UIFactory));
	// 화면에 보이게 하고싶다 
	Lv1UI->AddToViewport();

	// 미디어 플레이어를 재생하고 싶다 
	/*Lv1UI->OpenSource*/
}

void ADoronkoGameMode::SetScore(int32 Point)
{
	GameScore += Point;
	if (GameScore >= 1200 && GameScore < 1210)
	{	// Lv.1 선풍기 소환 & 컷씬 영상 추가 재생방지 
		if (countLv1 == 0) {
			SpawnWingFan();
			bLv1 = true;
		}
		countLv1++;
	}

	if (GameScore > 2400)
	{	// 컷씬 영상 추가 재생방지 
		if (countLv2 == 0) {
		// Lv.2 기차바퀴 소환 
		SpawnTrainWheel();
		bLv2 = true;
		}
		countLv2++;
	}

	if (GameScore > 3600)
	{ 
		if (countLv3 == 0) {
			// Lv.3 고래모자 소환 
			SpawnWhaleHat();
			bLv3 = true;
		}
		countLv3++;
	}

	if (GameScore > 4800)
	{	// Lv.4 기차바퀴 소환 & 컷씬 영상 추가 재생방지 
		if (countLv3 == 0) {
			// Lv.3 고래모자 소환 
			SpawnWhaleHat();
			bLv3 = true;
		}
		countLv3++;
		// Lv.4 기차바퀴(2) 소환 
		SpawnTrainWheel2();
	}

	if (GameScore > 5000)
	{
		// Lv.5 와인버튼 소환 
		SpawnWineButton();
	}
}

void ADoronkoGameMode::UpdataScoreBoard()
{
	ScoreBoard->SetText(GameScore);
}

void ADoronkoGameMode::SpawnWingFan()
{
	// 선풍기 소환 
	Transform1.SetLocation(FVector(-936, 623, 319));
	Transform1.SetRotation(FQuat(FRotator(0, 180, 0)));
	Transform1.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	GetWorld()->SpawnActor<AHJ_ElectricFan>(ElectricFanFactory, Transform1);

	// 레벨 작업 완료되면 컷씬 추가 필요 
}

void ADoronkoGameMode::SpawnTrainWheel()
{
	if (bTrainWheel1)
	{
		// 기차바퀴 소환
		Transform2.SetLocation(FVector(-1491, -4963, -431));
		Transform2.SetRotation(FQuat(FRotator(0, 0, 0)));
		Transform2.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, Transform2);
	}
	// Player가 물어가면, 추가 Spawn 방지 
	bTrainWheel1 = false;
}

void ADoronkoGameMode::SpawnWhaleHat()
{
	if (bWhaleHat)
	{
		// 고래모자 소환
		Transform3.SetLocation(FVector(-2358, -1884, 932));
		Transform3.SetRotation(FQuat(FRotator(0, 0, 0)));
		Transform3.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		GetWorld()->SpawnActor<AHJ_WhaleHat>(HatFactory, Transform3);
	}
	// Player가 물어가면, 추가 Spawn 방지 
	bWhaleHat = false;
}

void ADoronkoGameMode::SpawnTrainWheel2()
{
	if (bTrainWheel2)
	{
		// 기차바퀴 소환(2)
		Transform4.SetLocation(FVector(-2508, -2443, 885));
		Transform4.SetRotation(FQuat(FRotator(0, 0, 0)));
		Transform4.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, Transform4);
	}
	// Player가 물어가면, 추가 Spawn 방지 
	bTrainWheel2 = false;
}

void ADoronkoGameMode::SpawnWineButton()
{
	// 와인버튼 소환 
	Transform5.SetLocation(FVector(-2532, -4966, -513));
	Transform5.SetRotation(FQuat(FRotator(0, 0, 0)));
	Transform5.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetWorld()->SpawnActor<AWIneButton>(ButtonFactory, Transform5);
}


