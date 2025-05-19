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
	if (LifeTime <= 0) {
		// UI 제거
		RemoveFromParent();
	}

	// 찾은 스탬프 개수에 따라 이미지 위젯들의 알파값 설정
	SetImageAlpha();
}

void UHG_EnterInstruction::SetText(FText Name)
{
	// 방 이름 설정
	RoomName->SetText(Name);
}

void UHG_EnterInstruction::SetImageAlpha()
{
	FSlateColor TempColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GM) {
		switch (GM->StampCount)
		{
		case 1:
			StampProgress1->SetBrushTintColor(TempColor);
			break;

		case 2:
			StampProgress1->SetBrushTintColor(TempColor);
			StampProgress2->SetBrushTintColor(TempColor);
			break;

		case 3:

			StampProgress1->SetBrushTintColor(TempColor);
			StampProgress2->SetBrushTintColor(TempColor);
			StampProgress3->SetBrushTintColor(TempColor);
			break;

		case 4:	
			StampProgress1->SetBrushTintColor(TempColor);
			StampProgress2->SetBrushTintColor(TempColor);
			StampProgress3->SetBrushTintColor(TempColor);
			StampProgress4->SetBrushTintColor(TempColor);
			break;

		case 5:
			StampProgress1->SetBrushTintColor(TempColor);
			StampProgress2->SetBrushTintColor(TempColor);
			StampProgress3->SetBrushTintColor(TempColor);
			StampProgress4->SetBrushTintColor(TempColor);
			StampProgress5->SetBrushTintColor(TempColor);
			break;
		}
	}
	else return;
}
