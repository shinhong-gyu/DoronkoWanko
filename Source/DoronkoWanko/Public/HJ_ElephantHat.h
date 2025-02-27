// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Player.h"
#include "MasterItem.h"
#include "HJ_ElephantHat.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_ElephantHat : public AMasterItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_ElephantHat();

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
	float MakeTime = 0.1f;

	float CurrTime;

	void InteractionWith() override;

	void ItemDrop() override;

	bool bTurnOn = false;

	AGW_Player* GW_Player;

	FLinearColor MyColor = FLinearColor(1.0f,0.5f,0.946f,1.0f);
};
