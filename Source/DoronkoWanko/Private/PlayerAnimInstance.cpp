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
	if (Montage_IsPlaying(RubMontage))
	{
		if (player && player->bIsRightMouseDown) // bIsRightMouseDown: ���콺 ������ ��ư�� �����ִ� ����
		{
			Montage_JumpToSection(FName("rub"), RubMontage); // "rub" �������� �����Ͽ� ���� ����
		}
	}

}

void UPlayerAnimInstance::PlaySplashMontage()
{

Montage_Play(SplashMontage);
		
}

void UPlayerAnimInstance::PlayRubMontage()
{
	Montage_Play(RubMontage);

	AGW_Player* Player = Cast<AGW_Player>(TryGetPawnOwner());
	if (Player)
	{
		Player->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // <><><> �̵������� ���� 
	}
}
void UPlayerAnimInstance::AnimNotify_End()
{
	AGW_Player* Player = Cast<AGW_Player>(TryGetPawnOwner());
	if (Player)
	{
		Player->GetCharacterMovement()->MaxWalkSpeed = Player->WalkSpeed;
	}
}
// void UPlayerAnimInstance::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
// {
// 	Super::OnNotifyBegin(NotifyName, BranchingPointPayload);
// 
// 	if (NotifyName == FName("rubStart"))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("rubStart Notify triggered"));
// 
// 		
// 	}
// }
