// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "HJ_FanWing.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_FanWing : public ADynamicObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHJ_FanWing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	float CurrentRotationAngel;

	void InteractionWith() override;

	bool bTurnOn = false;

	// 선풍기 충돌체 소환 
	UPROPERTY(EditAnywhere)
	class TSubclassOf<class AHJ_FanWingSplatter> WingSplatter;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* WingArrow;

	AHJ_FanWingSplatter* SpawnedSplatter;

	// 선풍기 소리 
	UPROPERTY(EditAnywhere)
	class USoundBase* FanWingSFX;
};
