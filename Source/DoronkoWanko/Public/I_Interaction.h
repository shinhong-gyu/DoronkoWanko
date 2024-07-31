// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Interaction : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class DORONKOWANKO_API II_Interaction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// E 가 눌렸을 때 할 행동
	virtual void IteractionWith() = 0;
	// Player 가 아이템을 바라보고 있을 때
	virtual void LookAt() = 0;
	// Player 가 아이템을 보다 다른 곳을 바라볼 때
	virtual void FadeAway() = 0;
	// 아이템 놓기
	virtual void ItemDrop() = 0;
};
