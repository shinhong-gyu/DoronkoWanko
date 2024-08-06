 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Player.h"
#include "HG_Splatter.h"
#include "HJ_FanWing.h"
#include "HJ_Splatter2.h"
#include "HJ_FanWingSplatter.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_FanWingSplatter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_FanWingSplatter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyActorBeginOverlap(AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* InkArrow;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_Splatter2> InkFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHJ_FanWing> HJ_FanWing;

	AHJ_FanWing* HJ_Fan;

	AGW_Player* GW_Player;

};
