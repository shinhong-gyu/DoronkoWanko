// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticObject.h"
#include "HG_Splatter.h"
#include "GW_Player.h"
#include "HJ_Wine1.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_Wine1 : public AStaticObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_Wine1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> InkFactory;

	void SpawnInk();

	AGW_Player* GW_Player;

	FTimerHandle TimerHandle;

	void InteractionWith() override;

	FVector Vec;
	FRotator Rot;
	int check = 0;

	FLinearColor MyColor = FLinearColor(0.4611f, 0.02f, 0.474f, 1.0f);

};
