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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UDecalComponent* Decal;

	UPROPERTY(EditAnywhere)
	class UMaterial* DecalMaterial;


	UPROPERTY(EditAnywhere)
	int32 StampID;
};
