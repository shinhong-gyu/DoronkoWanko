// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_Player.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "I_Interaction.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetSystemLibrary.h"
#include "HG_Splatter.h"

// Sets default values
AGW_Player::AGW_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetArmLength = 300.0f;
	ZoomSpeed = 75.0f;
	MinArmLength = 50.0f;
	MaxArmLength = 1000.0f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = TargetArmLength;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
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

	FHitResult OutHit;
	FVector Start = this->GetActorLocation();
	FVector End = Start + CameraComp->GetForwardVector() + 1000.f;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params);
	if (bHit) {
		// 바라본 곳에 뭔가 있다.
		if (OutHit.GetActor() != LookAtActor) {
			LookAtActor = OutHit.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetClass()->GetName())
			II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
			if (Interface) {
				Interface->LookAt();
			}
		}
		DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3);

	}
	else {
		if (IsValid(LookAtActor)) {
			II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
			if (Interface) {
				Interface->FadeAway();
				LookAtActor = nullptr;
			}
		}
		// 허공
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3);
	}
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
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &AGW_Player::OnMyActionJump);
		input->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionZoom);
		input->BindAction(IA_Dash, ETriggerEvent::Ongoing, this, &AGW_Player::OnMyActionDashOngoing);
		input->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AGW_Player::OnMyActionDashCompleted);
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
	Shake();
	int NumberOfSplatter = FMath::RandRange(3,5);
	UE_LOG(LogTemp, Warning, TEXT("%d"), NumberOfSplatter)
	for (int i = 0; i < NumberOfSplatter; i++) {
		Shake();
	}
}

void AGW_Player::OnMyActionZoom(const FInputActionValue& Value)
{
	const float ZoomAmount = Value.Get<float>() * ZoomSpeed;
	TargetArmLength = FMath::Clamp(TargetArmLength + ZoomAmount, MinArmLength, MaxArmLength);
	SpringArmComp->TargetArmLength = TargetArmLength;
}

// void AGW_Player::OnMyActionDashOngoing(const FInputActionValue& Value, ETriggerEvent TriggerEvent)
// {
// 	if (TriggerEvent == ETriggerEvent::Started)
// 	{
// 		// 대시가 시작될 때 실행되는 코드
// 		GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
// 	}
// 	else if (TriggerEvent == ETriggerEvent::Completed)
// 	{
// 		// 대시가 완료될 때 실행되는 코드
// 		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
// 	}
// }

void AGW_Player::OnMyActionDashOngoing(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
}

void AGW_Player::OnMyActionDashCompleted(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

}

void AGW_Player::Shake()
{
	UE_LOG(LogTemp, Warning, TEXT("Shake"))
	FVector InitialVelocity = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(300, 600));

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	auto* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, SpawnLocation, SpawnRotation);

	if (Splatter) {
		Splatter->Initalize(InitialVelocity);
	}
}




