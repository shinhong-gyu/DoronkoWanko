// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "HJ_ElectricFan.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_ElectricFan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_ElectricFan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;


	/*UPROPERTY(EditAnywhere)
	class TSubclassOf<class AHJ_FanWingSplatter> WingSplatter;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* WingArrow; */

};