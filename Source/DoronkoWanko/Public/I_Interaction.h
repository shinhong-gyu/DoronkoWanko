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
	// E �� ������ �� �� �ൿ
	virtual void IteractionWith() = 0;
	// Player �� �������� �ٶ󺸰� ���� ��
	virtual void LookAt() = 0;
	// Player �� �������� ���� �ٸ� ���� �ٶ� ��
	virtual void FadeAway() = 0;
	// ������ ����
	virtual void ItemDrop() = 0;
};
