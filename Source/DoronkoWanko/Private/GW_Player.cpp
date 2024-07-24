// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_Player.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
AGW_Player::AGW_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGW_Player::BeginPlay()
{
	Super::BeginPlay();

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}

	
}

// Called every frame
void AGW_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform ttt = FTransform(GetControlRotation());
	Direction = ttt.TransformVector(Direction);
	AddMovementInput(Direction, 1);
	Direction = FVector::ZeroVector;


}

// Called to bind functionality to input
void AGW_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionMove);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionLook);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionJump);

	}

}

void AGW_Player::OnMyActionMove(const FInputActionValue& Value)
{

	{
		FVector2D v = Value.Get<FVector2D>();
		Direction.X = v.X;
		Direction.Y = v.Y;
		Direction.Normalize();
	}
}

void AGW_Player::OnMyActionLook(const FInputActionValue& Value)
{

	FVector2D v = Value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);

}

void AGW_Player::OnMyActionJump(const FInputActionValue& Value)
{
	Jump();
}

