// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
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
	float MoveCheck = 0;

	void Rotate();

	// 플레이어 입력 키 받기 
	int PressE;

	FTimerHandle TimerHandle;



};
