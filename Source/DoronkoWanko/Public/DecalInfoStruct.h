// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalInfoStruct.generated.h"

UCLASS()
class DORONKOWANKO_API ADecalInfoStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalInfoStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic,BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere)
	FVector Pos;

	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	FDecalInfo() :
		DecalComp(nullptr), Pos(FVector::ZeroVector), Color(FLinearColor::White)
	{}
};


