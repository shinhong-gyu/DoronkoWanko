// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterInstruction.h"
#include "Components/TextBlock.h"

void UHG_EnterInstruction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	LifeTime -= InDeltaTime;
	if (LifeTime <= 0) {
		RemoveFromParent();
	}
}

void UHG_EnterInstruction::SetText(FText Name)
{
	UE_LOG(LogTemp,Warning,TEXT("aaaa"));
	RoomName->SetText(Name);
}
