// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HG_EnterInstruction.generated.h"

/**
 *
 */
UCLASS()
class DORONKOWANKO_API UHG_EnterInstruction : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RoomName;

	void SetText(FText Name);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LifeTime = 2.0f;

};
