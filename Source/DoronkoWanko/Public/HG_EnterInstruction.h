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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* StampProgress1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* StampProgress2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* StampProgress3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* StampProgress4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* StampProgress5;

	void SetText(FText Name);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LifeTime = 2.0f;

	void SetImageAlpha();

};
