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

	//앞뒤 속도
	Speed = FVector::DotProduct(forwardVector, velocity);
	//좌우 속도
	Direction = FVector::DotProduct(rightVector, velocity);

	IsInAir = player->GetCharacterMovement()->IsFalling();
	if (Montage_IsPlaying(RubMontage))
	{
		if (player && player->bIsRightMouseDown) // bIsRightMouseDown: 마우스 오른쪽 버튼이 눌려있는 상태
		{
			Montage_JumpToSection(FName("rub"), RubMontage); // "rub" 섹션으로 점프하여 루프 시작
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
		Player->GetCharacterMovement()->MaxWalkSpeed = 0.0f; // <><><> 이동문제로 수정 
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
