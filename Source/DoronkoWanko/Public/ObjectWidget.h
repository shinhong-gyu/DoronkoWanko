// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class DORONKOWANKO_API UObjectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractionText;

	UFUNCTION()
	void SetText(FText s);
};
