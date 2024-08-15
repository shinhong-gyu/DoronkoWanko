// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "HG_Splatter.h"
#include "HJ_RoboticVacuum.generated.h"

UCLASS()
class DORONKOWANKO_API ARoboticVacuum : public ADynamicObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoboticVacuum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/*UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;*/

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	float CurrentRotationAngle;
	// 회전 
	FRotator CheckAngle;

	float Speed = 300;

	float Check = 0;

	void Rotate();

	FTimerHandle TimerHandle;

	bool bTurnOn = false;

	void InteractionWith() override;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ColorComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ColorComp2;

	void TurnOn();

	// 페인트 스폰 
	UPROPERTY(EditAnywhere)
	float MakeTime = 0.25;

	float CurrTime = 0;
	int SpawnCheck = 0;
	int SpawnTimeCheck = 0;
	int StopCheck = 0;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	// 청소기 작동 소리 
	UPROPERTY(EditAnywhere)
	class USoundBase* VacuumSFX;


	FLinearColor MyColor;
};
