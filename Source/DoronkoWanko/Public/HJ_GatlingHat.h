// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Player.h"
#include "MasterItem.h"
#include "HJ_GatlingHat.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_GatlingHat : public AMasterItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_GatlingHat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// ��ũ �߻��ϱ� 
	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	void SpawnInk();

	UPROPERTY(EditAnywhere)
	float MakeTime = 0.15f;

	float CurrTime;

	AGW_Player* GW_Player;
	// �������̽� ���
	void InteractionWith() override;

	void ItemDrop() override;

	bool bTurnOn = false;



};
