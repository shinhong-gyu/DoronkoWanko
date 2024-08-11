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

	// BGM
	UPROPERTY(EditAnywhere)
	class USoundBase* BGM;
	void PlayBGM();
	FTimerHandle BGMHandler;

	// 점수 UI 
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv1 = false;
	int countLv1 = 0;

	// 열차 바퀴 소환 
	void SpawnTrainWheel();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AHJ_TrainWheel> WheelFactory;
	FTransform Transform2;
	// 추가 스폰 방지 
	bool bTrainWheel1 = true;
	// 추가 컷신 재생방지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv2 = false;
	int countLv2 = 0;

	// 고래 모자 소환 
	void SpawnWhaleHat();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_WhaleHat> HatFactory;
	FTransform Transform3;
	bool bWhaleHat = true;
	// 추가 컷신 재생방지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv3 = false;
	int countLv3 = 0;

	// 열차 바퀴 소환 
	void SpawnTrainWheel2();
	FTransform Transform4;
	bool bTrainWheel2 = true;
	// 추가 컷신 재생방지 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv4 = false;
	int countLv4 = 0;

	// 와인버튼 소환 
	void SpawnWineButton();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWIneButton> ButtonFactory;
	FTransform Transform5;

	// 시퀀스 UI 소환 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory;

	UPROPERTY()
	class UWBP_Doronko_Lv1* Lv1UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory2;

	UPROPERTY()
	class UWBP_Doronko_Lv2* Lv2UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory3;

	UPROPERTY()
	class UWBP_Doronko_Lv3* Lv3UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory4;

	UPROPERTY()
	class UWBP_Doronko_Lv4* Lv4UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* itemSFX;
};
