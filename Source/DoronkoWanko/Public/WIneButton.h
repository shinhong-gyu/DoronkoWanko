// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "GW_Player.h"
#include "WIneButton.generated.h"

UCLASS()
class DORONKOWANKO_API AWIneButton : public ADynamicObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWIneButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Red;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Green;

	AGW_Player* GW_Player;

	void InteractionWith() override;

	// 페인트 스폰 

	void Shake();

	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	UPROPERTY(EditAnywhere)
	bool bFinal = true;

};
