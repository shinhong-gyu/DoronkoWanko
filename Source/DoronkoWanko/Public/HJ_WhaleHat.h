// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Player.h"
#include "MasterItem.h"
#include "HJ_WhaleHat.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_WhaleHat : public AMasterItem
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
	// 잉크 발사 
	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	void SpawnInk();

	UPROPERTY(EditAnywhere)
	float MakeTime = 0.2f;

	float CurrTime;
	// 인터페이스 함수 
	void InteractionWith() override;

	void ItemDrop() override;

	bool bTurnOn = false;

	AGW_Player* GW_Player;


	FLinearColor MyColor = FLinearColor(0.13f, 0.546f, 0.888f, 1.0f);

};
