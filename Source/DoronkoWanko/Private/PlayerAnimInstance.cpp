// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GW_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	AGW_Player* player = Cast<AGW_Player>(TryGetPawnOwner());
	if (nullptr == player)
	{
		return;
	}
	FVector velocity = player->GetVelocity();
	FVector forwardVector = player->GetActorForwardVector();
	FVector rightVector = player->GetActorRightVector();

	//�յ� �ӵ�
	Speed = FVector::DotProduct(forwardVector, velocity);
	//�¿� �ӵ�
	Direction = FVector::DotProduct(rightVector, velocity);

	IsInAir = player->GetCharacterMovement()->IsFalling();
}