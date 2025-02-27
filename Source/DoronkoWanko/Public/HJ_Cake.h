// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicObject.h"
#include "GW_Player.h"
#include "HJ_Cake.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_Cake : public ADynamicObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHJ_Cake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AGW_Player* GW_Player;

	void InteractionWith() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> EndingUIFactory;

	UPROPERTY()
	class UHJ_EndingWidget* EndingUI;
};
