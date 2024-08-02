// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DORONKOWANKO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;
	//Horizontal
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Direction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsInAir;



};
