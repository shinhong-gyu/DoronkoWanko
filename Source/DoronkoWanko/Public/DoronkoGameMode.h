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
	class USoundBase* BGM;
	void PlayBGM();
	FTimerHandle BGMHandler;

	// ���� UI 
	int32 GameScore =600; // ���� �ʿ� 

	void SetScore(int32 Point);

	void UpdataScoreBoard();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHG_ScoreBoard> ScoreBoardFactory;

	UPROPERTY(EditAnywhere)
	class UHG_ScoreBoard* ScoreBoard;

	// ��ǳ�� ��ȯ 
	void SpawnWingFan();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_ElectricFan> ElectricFanFactory;
	FTransform Transform1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv1 = false;
	int countLv1 = 0;

	// ���� ���� ��ȯ 
	void SpawnTrainWheel();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AHJ_TrainWheel> WheelFactory;
	FTransform Transform2;
	// �߰� ���� ���� 
	bool bTrainWheel1 = true;
	// �߰� �ƽ� ������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv2 = false;
	int countLv2 = 0;

	// �� ���� ��ȯ 
	void SpawnWhaleHat();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_WhaleHat> HatFactory;
	FTransform Transform3;
	bool bWhaleHat = true;
	// �߰� �ƽ� ������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv3 = false;
	int countLv3 = 0;

	// ���� ���� ��ȯ 
	void SpawnTrainWheel2();
	FTransform Transform4;
	bool bTrainWheel2 = true;
	// �߰� �ƽ� ������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv4 = false;
	int countLv4 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLv5 = false;
	int countLv5 = 0;

	int StampCount = 0;

	// ���ι�ư ��ȯ 
	void SpawnWineButton();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWIneButton> ButtonFactory;
	FTransform Transform5;


	// ������ UI ��ȯ 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> UIFactory;

	// ���� �ƾ� ���� 
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

	// ��Ÿ �ƾ� ���� 
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
	class USoundBase* itemSFX;

public:

	TArray<FDecalInfo> SpawnedDecalArr;
};
