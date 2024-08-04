// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_ScoreBoard.h"
#include "Components/TextBlock.h"

void UHG_ScoreBoard::SetText(int32 value)
{
	Score->SetText(FText::AsNumber(value));
}
