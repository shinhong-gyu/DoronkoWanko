// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_ScoreBoard.generated.h"

/**
 *
 */
UCLASS()
class DORONKOWANKO_API UHG_ScoreBoard : public UUserWidget
{
	GENERATED_BODY()
public:
	// 스코어보드의 점수를 설정하기 위한 함수.
	void SetText(int32 value);

	// 스코어보드의 점수 텍스트 위젯
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Score;
};
