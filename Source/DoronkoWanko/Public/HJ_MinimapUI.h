// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HJ_MinimapUI.generated.h"

/**
 * 
 */
UCLASS()
class DORONKOWANKO_API UHJ_MinimapUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowFloor(int32 Floor);

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Base;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_First;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Second;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_four;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Five;

	void MiniMapUpdate(int32 StampID);


};
