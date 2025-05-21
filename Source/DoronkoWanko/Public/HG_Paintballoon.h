// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "HG_Paintballoon.generated.h"

UCLASS()
class DORONKOWANKO_API AHG_Paintballoon : public ADynamicObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_Paintballoon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// InteractionWith 함수 오버라이드
	UFUNCTION()
	virtual void InteractionWith() override;

	// 페인트 공의 색
	FLinearColor MyColor;

	// 팩토리 패턴
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> SplatterFactory;
};
