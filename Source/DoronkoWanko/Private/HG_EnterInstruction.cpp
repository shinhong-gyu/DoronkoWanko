// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterInstruction.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DoronkoGameMode.h"
#include "Styling/SlateColor.h"

void UHG_EnterInstruction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 생명주기 감소
	LifeTime -= InDeltaTime;

	// 만약 생명주기가 0보다 작거나 같아지면
	if (LifeTime <= 0)
	{
		// UI 제거
		RemoveFromParent();
	}

	// 찾은 스탬프 개수에 따라 이미지 위젯들의 알파값 설정
	UpdateStampProgress();
}

void UHG_EnterInstruction::SetRoomName(FText Name)
{
	// 방 이름 설정
	if (TB_RoomName)
	{
		TB_RoomName->SetText(Name);
	}
}

void UHG_EnterInstruction::UpdateStampProgress()
{
	// 이미지에 설정할 색상 값
	FSlateColor ColorValue = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 현재 게임 모드 가져오기
	ADoronkoGameMode* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	// 게임모드가 유효하다면
	if (nullptr != GM)
	{
		// 찾은 스탬프의 개수에 따라 이미지의 색 설정
		switch (GM->StampCount)
		{
		case 1:
			StampProgress1->SetBrushTintColor(ColorValue);
			break;

		case 2:
			StampProgress1->SetBrushTintColor(ColorValue);
			StampProgress2->SetBrushTintColor(ColorValue);
			break;
		case 3:
			StampProgress1->SetBrushTintColor(ColorValue);
			StampProgress2->SetBrushTintColor(ColorValue);
			StampProgress3->SetBrushTintColor(ColorValue);
			break;
		case 4:
			StampProgress2->SetBrushTintColor(ColorValue);
			StampProgress3->SetBrushTintColor(ColorValue);
			StampProgress1->SetBrushTintColor(ColorValue);
			StampProgress4->SetBrushTintColor(ColorValue);
			break;
		case 5:
			StampProgress1->SetBrushTintColor(ColorValue);
			StampProgress2->SetBrushTintColor(ColorValue);
			StampProgress3->SetBrushTintColor(ColorValue);
			StampProgress4->SetBrushTintColor(ColorValue);
			StampProgress5->SetBrushTintColor(ColorValue);
			break;
		}
	}
	else
	{
		return;
	}
}
