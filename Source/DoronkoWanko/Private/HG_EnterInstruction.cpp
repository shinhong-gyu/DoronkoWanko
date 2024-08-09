// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_EnterInstruction.h"
#include "Components/TextBlock.h"

void UHG_EnterInstruction::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	LifeTime -= InDeltaTime;
	if (LifeTime <= 0) {
		this->RemoveFromParent();
	}
}

void UHG_EnterInstruction::SetText(FText Name)
{
	RoomName->SetText(Name);
}
