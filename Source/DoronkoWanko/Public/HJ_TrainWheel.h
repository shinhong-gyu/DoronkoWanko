// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "HJ_TrainWheel.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_TrainWheel : public AStaticObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_TrainWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// MasterInteraction 에서 정의 
	/*UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;*/

};
