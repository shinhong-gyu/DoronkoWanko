// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "GW_Player.h"
#include "HG_Splatter.h"
#include "HJ_Train.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_Train : public ADynamicObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_Train();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Wheel1;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Wheel2;

	UPROPERTY(EditAnywhere)
	int WheelCheck = 0;

	/*UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Wheel3;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Wheel4;*/

	/*UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;*/

	// ���Ͽ� ������ ���� Ŭ���� ���� 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_TrainWheel> WheelFactory;

	class AHJ_TrainWheel* CurrentWheel;

	// ������ ��ü(��ü) ȸ�� ���� 
	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float AngularSpeed;

	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	// ������ ���� ȸ�� ���� 
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	float CurrentRotationAngel;

	// ��ũ ����
	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	// ���� �ð�
	float CurrTime = 0;
	
	UPROPERTY(EditAnywhere)
	float MakeTime = 0.3f;

	// ���ͷ���
	bool bTurnOn = false;

	void InteractionWith() override;

	void ItemDrop() override;

	AGW_Player* GW_Player;

};
