// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HJ_ElectricFan.h"
#include "HJ_TrainWheel.h"
#include "HJ_WhaleHat.h"
#include "WIneButton.h"
#include "DoronkoGameMode.generated.h"

/**
 *
 */
UCLASS()
class DORONKOWANKO_API ADoronkoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	int32 GameScore = 0;

	void SetScore(int32 Point);

	void UpdataScoreBoard();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHG_ScoreBoard> ScoreBoardFactory;

	UPROPERTY(EditAnywhere)
	class UHG_ScoreBoard* ScoreBoard;

	// 선풍기 소환 
	void SpawnWingFan();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_ElectricFan> ElectricFanFactory;
	FTransform Transform1;

	// 열차 바퀴 소환 
	void SpawnTrainWheel();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_TrainWheel> WheelFactory;
	FTransform Transform2;
	bool bTrainWheel1 = true;

	// 고래 모자 소환 
	void SpawnWhaleHat();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_WhaleHat> HatFactory;
	FTransform Transform3;
	bool bWhaleHat = true;

	// 열차 바퀴 소환 
	void SpawnTrainWheel2();
	FTransform Transform4;
	bool bTrainWheel2 = true;

	// 와인버튼 소환 
	void SpawnWineButton();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWIneButton> ButtonFactory;
	FTransform Transform5;







};
