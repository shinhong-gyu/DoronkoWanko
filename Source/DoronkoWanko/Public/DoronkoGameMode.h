// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HJ_ElectricFan.h"
#include "HJ_TrainWheel.h"
#include "HJ_WhaleHat.h"
#include "WIneButton.h"
#include "Components/DecalComponent.h"
#include "DecalInfoStruct.h"
#include "WBP_Doronko_CLv5.h"
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
	TObjectPtr<class USoundBase> BGM;

	// BGM 재생 함수
	void PlayBGM();

	// BGM을 반복시키기 위한 TimerHandle
	FTimerHandle BGMHandler;

	// 점수 UI 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameScore = 0; // 변경 필요 

	// 최대 점수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScore;

	// 점수 세팅하는 함수
	void AddScore(int32 Point);

	// UI 업데이트 함수
	void UpdataScoreBoard();

	// ScoreBoard를 생성하기 위한 스코어보드 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHG_ScoreBoard> ScoreBoardClass;


	// 생성한 스코어보드를 저장하기위한 변수
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHG_ScoreBoard> ScoreBoard;

	// 선풍기 소환 함수
	void SpawnWingFan();

	// 선풍기를 생성하기 위한 선풍기 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_ElectricFan> ElectricFanClass;

	// 선풍기 소환을 위한 트랜스폼
	FTransform FanSpawnTransform;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv5 = false;
	int countLv5 = 0;

	int StampCount = 0;

	// 와인버튼 소환 
	void SpawnWineButton();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWIneButton> ButtonFactory;
	FTransform Transform5;


	// 시퀀스 UI 소환 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory;

	// 알파 컷씬 위젯 
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

	// 베타 컷씬 위젯 
	UPROPERTY()
	class UWBP_Doronko_CLv1* CLv1UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactoryC1;

	UPROPERTY()
	class UWBP_Doronko_CLv2* CLv2UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactoryC2;

	UPROPERTY()
	class UWBP_Doronko_CLv3* CLv3UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactoryC3;

	UPROPERTY()
	class UWBP_Doronko_CLv4* CLv4UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactoryC4;

	UPROPERTY()
	class UWBP_Doronko_CLv5* CLv5UI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactoryC5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* ItemSFX;

public:
	TArray<FDecalInfo> SpawnedDecalArr;
};
