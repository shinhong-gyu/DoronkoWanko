// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "HJMiniMapWidget.generated.h"

/**
 *
 */
UCLASS()
class DORONKOWANKO_API UHJMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowFloor(int32 Floor);

	UPROPERTY(meta = (BindWidget))
	UImage* Image_0;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_1;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_2;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_3;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_4;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_5;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_6;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_7;

	int32 zaZOrder;
	int32 zbZOrder;
	int32 zcZOrder;
	int32 zdZOrder;
	int32 zeZOrder;
	int32 zfZOrder;
	int32 zgZOrder;
	int32 zhZOrder;

	UCanvasPanelSlot* saSlot;
	UCanvasPanelSlot* sbSlot;
	UCanvasPanelSlot* scSlot;
	UCanvasPanelSlot* sdSlot;
	UCanvasPanelSlot* seSlot;
	UCanvasPanelSlot* sfSlot;
	UCanvasPanelSlot* sgSlot;
	UCanvasPanelSlot* shSlot;

};
