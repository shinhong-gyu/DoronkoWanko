// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJ_ElectricFan.h"
#include "HJ_WhaleHat.h"
#include "HJ_TrainWheel.h"
#include "WIneButton.h"
#include "HJ_Box.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_Box : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_Box();

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

	// �ٴڿ� ��ġ�� �浹ü�� �浹�� ������� 
	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector Vec;
	FRotator Rot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_ElectricFan> ElectricFanFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_WhaleHat> HatFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_TrainWheel> WheelFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWIneButton> ButtonFactory;
};
