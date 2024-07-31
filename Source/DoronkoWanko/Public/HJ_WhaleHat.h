// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "HJ_WhaleHat.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_WhaleHat : public ADynamicObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_WhaleHat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	void SpawnInk();

	UPROPERTY(EditAnywhere)
	float MakeTime = 0.2f;

	float CurrTime;

	int PressE = 0;

};
