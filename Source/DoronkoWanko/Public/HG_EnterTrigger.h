// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Player.h"
#include "HJ_MinimapUI.h"
#include "HG_EnterTrigger.generated.h"

UCLASS()
class DORONKOWANKO_API AHG_EnterTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_EnterTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 플레이어가 방에 들어갈 때 발생할 일을 처리하는 함수
	void EnterTriggered();

	// 충돌체
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComp;

	// 트리거가 나타내는 방의 이름을 저장하는 변수
	UPROPERTY(EditAnywhere)
	FText RoomName;

	// 플레이어가 Collision을 발생시켰을 때 호출될 함수
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 현재 플레이어가 위치한 방의 정보를 저장하는 변수 
	UPROPERTY(EditAnywhere)
	EPlayerRoomState CurrentRoomState;
	
};
