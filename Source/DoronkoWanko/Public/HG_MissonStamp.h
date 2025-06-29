// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HG_MissonStamp.generated.h"

UCLASS()
class DORONKOWANKO_API AHG_MissonStamp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHG_MissonStamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 데칼 컴포넌트
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDecalComponent> Decal;

	// 데칼 머터리얼
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> DecalMaterial;

	// 스탬프를 구분하기 위한 아이디
	UPROPERTY(EditAnywhere)
	int32 StampID;
};
