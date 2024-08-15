// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterInstruction.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DoronkoGameMode.h"
#include "Styling/SlateColor.h"

void UHG_EnterInstruction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	LifeTime -= InDeltaTime;
	if (LifeTime <= 0) {
		RemoveFromParent();
	}
	SetImageAlpha();
}

void UHG_EnterInstruction::SetText(FText Name)
{
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
			UE_LOG(LogTemp, Warning, TEXT("1"));
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
