// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_EnterInstruction.generated.h"

/**
 *
 */

class UImage;
class UTextBlock;

UCLASS()
class DORONKOWANKO_API UHG_EnterInstruction : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 방 이름을 나타내는 텍스트 블록
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_RoomName;

	// 스탬프 진행 상황을 나타내는 이미지
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> StampProgress1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> StampProgress2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> StampProgress3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> StampProgress4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> StampProgress5;

	// 스탬프 발견 진행 상황을 업데이트 하는 함수
	void UpdateStampProgress();

	// 방의 이름을 설정하는 함수
	void SetRoomName(FText Name);

	// 위젯의 생명 주기를 나타내는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTime = 2.0f;

	
};
