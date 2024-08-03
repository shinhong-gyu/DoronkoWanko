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
#include "Components/CapsuleComponent.h"
#include "HG_Splatter.h"
#include "helmet.h"
#include "DynamicObject.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MasterItem.h"

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

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
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
	Direction.Z = 0;
	Direction.Normalize();
	AddMovementInput(Direction, 1);
	Direction = FVector::ZeroVector;
	FHitResult OutHit;
	FVector Start = GetActorLocation() - FVector(0, 0, -36.666f);
	FVector End = Start + GetActorForwardVector() * 1000;
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 150.0f, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true);
	if (bHit) {
		// 바라본 곳에 뭔가 있다.
		UE_LOG(LogTemp, Warning, TEXT("1"));
		if (LookAtActor == nullptr) {
			if (OutHit.GetActor() != LookAtActor) {
				LookAtActor = OutHit.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("LookAt : %s"), *LookAtActor->GetClass()->GetName());
				UE_LOG(LogTemp, Warning, TEXT("%s"), *OutHit.GetActor()->GetClass()->GetName());
				II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
				if (Interface) {
					Interface->LookAt();
				}
			}
		}
	}
	else {
		II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
		if (Interface) {
			Interface->FadeAway();
			LookAtActor = nullptr;
		}

	}
	SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetArmLength, DeltaTime, ZoomSpeed);
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
		input->BindAction(IA_Dash, ETriggerEvent::Started, this, &AGW_Player::OnMyActionDashOngoing);
		input->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AGW_Player::OnMyActionDashCompleted);
		input->BindAction(IA_Interaction, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionInteraction);
		input->BindAction(IA_Drop, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionDrop);
		input->BindAction(IA_Splash, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionSplash);
		input->BindAction(IA_Dirt, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionDirt);
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
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
}

void AGW_Player::OnMyActionDashCompleted(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;


}

void AGW_Player::Shake()
{
	FVector InitialVelocity = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(300, 600));

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	auto* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, SpawnLocation, SpawnRotation);

	if (Splatter) {
		Splatter->Initalize(InitialVelocity);
	}

}
void AGW_Player::OnMyActionDirt(const FInputActionValue& Value)
{
	FColor NewColor = FColor::MakeRandomColor();
	ColorArray.Add(NewColor);

	if (GEngine)
	{
		// 배열의 모든 항목을 화면에 표시
		for (int32 i = 0; i < ColorArray.Num(); i++)
		{
			FString Message = FString::Printf(TEXT("Color[%d]: %s"), i, *ColorArray[i].ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ColorArray[i], Message);
		}
	}
}


void AGW_Player::OnMyActionSplash(const FInputActionValue& Value)
{
	Shake();


	if (ColorArray.Num() > 0)
	{
		ColorArray.RemoveAt(ColorArray.Num() - 1);
	}

	if (GEngine)
	{
		for (int32 i = 0; i < ColorArray.Num(); i++)
		{
			FString Message = FString::Printf(TEXT("Color[%d]: %s"), i, *ColorArray[i].ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, ColorArray[i], Message);
		}

		if (ColorArray.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Color array is empty"));
		}
	}

}

void AGW_Player::OnMyActionInteraction(const FInputActionValue& Value)
{
	if (LookAtActor != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetClass()->GetName())
			II_Interaction* Interact = Cast<II_Interaction>(LookAtActor);
		if (Interact != nullptr) {
			Interact->InteractionWith();

			AMasterItem* DynamicObject = Cast<AMasterItem>(LookAtActor);
			if (DynamicObject)
			{

				attachDynamicObject();

			}

		}
	}
}

void AGW_Player::OnMyActionDrop(const FInputActionValue& Value)
{
	if (AttachedDOb != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *AttachedDOb->GetClass()->GetName());
		auto* Interact = Cast<II_Interaction>(AttachedDOb);
		if (Interact != nullptr)
		{
			Interact->ItemDrop();
			dropDynamicObject();
		}
		AttachedDOb = nullptr;
	}
	dropDynamicObject();
}

void AGW_Player::attachDynamicObject()
{
	OverlappingDObject = LookAtActor;
	if (OverlappingDObject && !AttachedDOb)
	{
		// Overlapping을 플레이어의 특정 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		OverlappingDObject->AttachToComponent(GetMesh(), AttachmentRules, FName("HAT"));
		OverlappingDObject->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X,
			1.0f / GetMesh()->GetComponentScale().Y, 1.0f / GetMesh()->GetComponentScale().Z));
		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(OverlappingDObject->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		AttachedDOb = OverlappingDObject;
		II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
		if (Interface) {
			Interface->FadeAway();
			LookAtActor = nullptr;
		}
	}
}

void AGW_Player::dropDynamicObject()
{

	if (AttachedDOb)
	{
		AttachedDOb->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(AttachedDOb->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			DObComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		}

		AttachedDOb = nullptr;
		OverlappingDObject = nullptr;
	}
}

void AGW_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMasterItem* dObject = Cast<AMasterItem>(OtherActor))
	{
		OverlappingDObject = dObject;
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *dObject->GetName());
	}
}

// void AGW_Player::OnMyActionInteraction(const FInputActionValue& Value)
// {
// 	if (OverlappingTrainWheel && !AttachedTrainWheel)
// 	{
// 		// OverlappingTrainWheel을 플레이어의 특정 소켓에 부착
// 		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
// 		OverlappingTrainWheel->AttachToComponent(GetMesh(), AttachmentRules, FName("attach"));
// 		OverlappingTrainWheel->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X,
// 			1.0f / GetMesh()->GetComponentScale().Y,1.0f / GetMesh()->GetComponentScale().Z));
// 		if (UPrimitiveComponent* TrainWheelComp = Cast<UPrimitiveComponent>(OverlappingTrainWheel->GetRootComponent()))
// 		{
// 			TrainWheelComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		}
// 		AttachedTrainWheel = OverlappingTrainWheel;
// 	}
// 	if (Overlappinghelmet && !Attachedhelmet)
// 	{
// 		// OverlappingTrainWheel을 플레이어의 특정 소켓에 부착
// 		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
// 		Overlappinghelmet->AttachToComponent(GetMesh(), AttachmentRules, FName("HAT"));
// 		Overlappinghelmet->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X,
// 			1.0f / GetMesh()->GetComponentScale().Y,1.0f / GetMesh()->GetComponentScale().Z));
// 		if (UPrimitiveComponent* HelmetComp = Cast<UPrimitiveComponent>(Overlappinghelmet->GetRootComponent()))
// 		{
// 			HelmetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		}
// 		Attachedhelmet = Overlappinghelmet;
// 	}
// 
// 
// }

// void AGW_Player::OnMyActionDrop(const FInputActionValue& Value)
// {
// 	if (AttachedTrainWheel)
// 	{
// 		AttachedTrainWheel->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		if (UPrimitiveComponent* TrainWheelComp = Cast<UPrimitiveComponent>(AttachedTrainWheel->GetRootComponent()))
// 		{
// 			TrainWheelComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 			TrainWheelComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
// 		}
// 		AttachedTrainWheel = nullptr;
// 		OverlappingTrainWheel = nullptr;
// 	}
// 	else if (Attachedhelmet)
// 	{
// 		Attachedhelmet->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		if (UPrimitiveComponent* HelmetComp = Cast<UPrimitiveComponent>(Attachedhelmet->GetRootComponent()))
// 		{
// 			HelmetComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 			HelmetComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
// 		}
// 
// 		Attachedhelmet = nullptr;
// 		Overlappinghelmet = nullptr;
// 	}
// }

// void AGW_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// // 	if (AHJ_TrainWheel* TrainWheel = Cast<AHJ_TrainWheel>(OtherActor))
// // 	{
// // 		OverlappingTrainWheel = TrainWheel;
// // 		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *TrainWheel->GetName());
// // 	}
// // 	else
// 	if (AHJ_TrainWheel* TrainWheel = Cast<AHJ_TrainWheel>(OtherActor))
// 	{
// 		OverlappingTrainWheel = TrainWheel;
// 		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *TrainWheel->GetName());
// 	}







