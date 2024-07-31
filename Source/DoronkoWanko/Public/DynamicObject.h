// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterMapObject.h"
#include "DynamicObject.generated.h"

/**
 * 
 */
UCLASS()
class DORONKOWANKO_API ADynamicObject : public AMasterMapObject
{
	GENERATED_BODY()

public:
	ADynamicObject();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
