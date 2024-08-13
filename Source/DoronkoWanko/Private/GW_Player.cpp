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
#include "PlayerAnimInstance.h"
#include "StaticObject.h"
#include "HG_EnterInstruction.h"

// Sets default values
AGW_Player::AGW_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetArmLength = 300.0f;
	ZoomSpeed = 75.0f;
	MinArmLength = 50.0f;
	MaxArmLength = 1000.0f;
	DirtPercentage = 0.0f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = TargetArmLength;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	AttachedMasterItem = nullptr;
	AttachedStaticObject = nullptr;
	OverlappingObject = nullptr;
// 	bIsDropping = false;
// 	ConstructorHelpers::FClassFinder<UPlayerAnimInstance> TempAnimInst(TEXT("'/Game/GyeongWon/Bp/ABP_Player.ABP_Player_C'"));
// 	if (TempAnimInst.Succeeded())
// 	{
// 		GetMesh()->SetAnimInstanceClass(TempAnimInst.Class);
// 	}
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
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		UE_LOG(LogTemp,Warning,TEXT("ANim"))
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
	FVector End = Start + GetActorForwardVector() * 100;
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 150.0f, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, true);
	if (bHit) {
		// 바라본 곳에 뭔가 있다.
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
// 	switch (LocState)
// 	{
// 	case EPlayerRoomState::KITCHEN:
// 		UE_LOG(LogTemp, Warning, TEXT("1"));
// 		break;
// 	case EPlayerRoomState::LIVINGROOM:
// 		UE_LOG(LogTemp, Warning, TEXT("2"));
// 		break;
// 	case EPlayerRoomState::BASEMENTLIVINGROOM:
// 		UE_LOG(LogTemp, Warning, TEXT("3"));
// 		break;
// 	case EPlayerRoomState::WINECELLAR:
// 		UE_LOG(LogTemp, Warning, TEXT("4"));
// 		break;
// 	case EPlayerRoomState::NURSERY:
// 		UE_LOG(LogTemp, Warning, TEXT("5"));
// 		break;
// 	default:
// 		break;
// 	}
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
		input->BindAction(IA_Interaction, ETriggerEvent::Started, this, &AGW_Player::OnMyActionInteraction);
		input->BindAction(IA_Drop, ETriggerEvent::Started, this, &AGW_Player::OnMyActionDrop);
		input->BindAction(IA_Splash, ETriggerEvent::Started, this, &AGW_Player::OnMyActionSplash);
		input->BindAction(IA_Dirt, ETriggerEvent::Started, this, &AGW_Player::OnMyActionDirtStart);
		input->BindAction(IA_Dirt, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionDirtOngoing);
		input->BindAction(IA_Dirt, ETriggerEvent::Completed, this, &AGW_Player::OnMyActionDirtEnd);
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
		int RandInt = FMath::RandRange(1,2);
		if (RandInt == 1) {
			Splatter->SetMyColor(FLinearColor::Red);
		}
		else {
			Splatter->SetMyColor(FLinearColor::White);
		}
		
	}

	
}
void AGW_Player::OnMyActionDirtStart(const FInputActionValue& Value)
{
	FColor NewColor = FColor::MakeRandomColor();
	ColorArray.Add(NewColor);

// 	if (GEngine)
// 	{
// 		// 배열의 모든 항목을 화면에 표시
// 		for (int32 i = 0; i < ColorArray.Num(); i++)
// 		{
// 			FString Message = FString::Printf(TEXT("Color[%d]: %s"), i, *ColorArray[i].ToString());
// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, ColorArray[i], Message);
// 		}
// 	}
	check(Anim)
		if (Anim)
		{
			Anim->PlayRubMontage();
			UE_LOG(LogTemp, Warning, TEXT("Rub"));
		}


}

void AGW_Player::OnMyActionDirtOngoing(const FInputActionValue& Value)
{
	if (DirtPercentage < 100.0f)
	{
		DirtPercentage += 5.0f;
		if (DirtPercentage > 100.0f)
		{
			DirtPercentage = 100.0f;
		}
	}

}

void AGW_Player::OnMyActionDirtEnd(const FInputActionValue& Value)
{
}


void AGW_Player::OnMyActionSplash(const FInputActionValue& Value)
{	
// 	UPlayerAnimInstance* anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	check(Anim)
	if (Anim)
	{
		Anim->PlaySplashMontage();
		UE_LOG(LogTemp, Warning, TEXT("splash"));
	}


	if (DirtPercentage > 0.0f)
	{
		DirtPercentage -= 0.25f;
		if (DirtPercentage < 0.0f)
		{
			DirtPercentage = 0.0f;
		}

		int NumberOfSplatter = FMath::RandRange(3, 5);
		for (int i = 0; i < NumberOfSplatter; i++) {
			Shake();
		}
	}
	else return;


}

void AGW_Player::OnMyActionInteraction(const FInputActionValue& Value)
{
	if (LookAtActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetClass()->GetName());

		II_Interaction* Interact = Cast<II_Interaction>(LookAtActor);
		if (Interact != nullptr)
		{
			Interact->InteractionWith();

			if (AMasterItem* MasterItem = Cast<AMasterItem>(LookAtActor))
			{
				HandleMasterItemAttachment(MasterItem);
			}
			else if (AStaticObject* DynamicObject = Cast<AStaticObject>(LookAtActor))
			{
				HandleStaticObjectAttachment(DynamicObject);
				UGameplayStatics::PlaySound2D(GetWorld(), Bite);
			}
		}
		II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
		LookAtActor = nullptr;
		if (Interface) {
			Interface->FadeAway();
			LookAtActor = nullptr;
		}

	}
	
}

void AGW_Player::OnMyActionDrop(const FInputActionValue& Value)
{
	

	if (AttachedStaticObject != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropping DynamicObject"));
		II_Interaction* Interact = Cast<II_Interaction>(AttachedStaticObject);
		if (Interact != nullptr)
		{
			Interact->ItemDrop();
		}

		dropObject(AttachedStaticObject);
		return;  // Return early after dropping DynamicObject
	}

	// Drop MasterItem if attached
	if (AttachedMasterItem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropping MasterItem"));
		II_Interaction* Interact = Cast<II_Interaction>(AttachedMasterItem);
		if (Interact != nullptr)
		{
			Interact->ItemDrop();
		}

		dropObject(AttachedMasterItem);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), Drop);

}
void AGW_Player::attachStaticicObject(AActor* ObjectToAttach)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);


	if (AMasterItem* MasterItem = Cast<AMasterItem>(ObjectToAttach))
	{
		// Attach MasterItem to "HAT" socket
		MasterItem->AttachToComponent(GetMesh(), AttachmentRules, FName("HAT"));
		MasterItem->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X,
			1.0f / GetMesh()->GetComponentScale().Y,
			1.0f / GetMesh()->GetComponentScale().Z));
		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(MasterItem->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		AttachedMasterItem = MasterItem;
// 		II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
// 		LookAtActor = nullptr;
// 		if (Interface) {
// 			Interface->FadeAway();
// 			LookAtActor = nullptr;
// 		}
	}
	else if (AStaticObject* StaticicObject = Cast<AStaticObject>(ObjectToAttach))
	{
		// Attach DynamicObject to "HAND" socket
		StaticicObject->AttachToComponent(GetMesh(), AttachmentRules, FName("attach"));
		StaticicObject->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X,
			1.0f / GetMesh()->GetComponentScale().Y,
			1.0f / GetMesh()->GetComponentScale().Z));
		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(StaticicObject->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		AttachedStaticObject = StaticicObject;
// 		II_Interaction* Interface = Cast<II_Interaction>(LookAtActor);
// 		LookAtActor = nullptr;
// 		if (Interface) {
// 			Interface->FadeAway();
// 			LookAtActor = nullptr;
// 		}
	}
}


void AGW_Player::dropObject(AActor* ObjectToDrop)
{

	if (ObjectToDrop)
	{
		ObjectToDrop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(ObjectToDrop->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			DObComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
			// 물리 시뮬레이션을 활성화 (필요한 경우)
// 			DObComp->SetSimulatePhysics(true);

			// 디버그 로그 추가
			UE_LOG(LogTemp, Warning, TEXT("Collision Enabled: %s"), *UEnum::GetValueAsString(DObComp->GetCollisionEnabled()));
		}

		if (ObjectToDrop == AttachedStaticObject)
		{
			AttachedStaticObject = nullptr;
		}
		else if (ObjectToDrop == AttachedMasterItem)
		{
			AttachedMasterItem = nullptr;
		}
	}
}

void AGW_Player::HandleMasterItemAttachment(AActor* ObjectToAttach)
{
	// 	if (AttachedDynamicObject != nullptr)
	// 	{
	// 		// Drop DynamicObject first if both are attached
	// 		dropDynamicObject(AttachedDynamicObject);
	// 	}

	if (AttachedMasterItem != nullptr)
	{
		II_Interaction* Interact = Cast<II_Interaction>(AttachedMasterItem);
		if (Interact != nullptr)
		{
			Interact->ItemDrop();
		}
		dropObject(AttachedMasterItem);
	}
	// Attach new MasterItem
	attachStaticicObject(ObjectToAttach);

	II_Interaction* NewInteract = Cast<II_Interaction>(ObjectToAttach);
	if (NewInteract != nullptr)
	{
		NewInteract->InteractionWith();
	}
}

void AGW_Player::HandleStaticObjectAttachment(AActor* ObjectToAttach)
{
	UE_LOG(LogTemp,Warning,TEXT("1"));
	// Drop existing DynamicObject if attached
	if (AttachedStaticObject != nullptr)
	{
		dropObject(AttachedStaticObject);
	}

	// Attach new DynamicObject
	attachStaticicObject(ObjectToAttach);

}

void AGW_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMasterItem* MasterItem = Cast<AMasterItem>(OtherActor))
	{
		OverlappingObject = MasterItem;
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *MasterItem->GetName());
	}
	else if (AStaticObject* StaticObject = Cast<AStaticObject>(OtherActor))
	{
		OverlappingObject = StaticObject;
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *StaticObject->GetName());
	}
	// 	else if (ADynamicObject* dObject = Cast<ADynamicObject>(OtherActor))
	// 	{
	// 		OverlappingDObject = dObject;
	// 		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *dObject->GetName());
	// 	}
}
void AGW_Player::SetLocState(EPlayerRoomState Loc)
{
	FText TempText;
	switch (Loc)
	{
	case EPlayerRoomState::KITCHEN:   TempText = FText::FromString(TEXT("Kitchen"));   break;
	case EPlayerRoomState::LIVINGROOM:   TempText = FText::FromString(TEXT("Living Room"));      break;
	case EPlayerRoomState::BASEMENTLIVINGROOM:   TempText = FText::FromString(TEXT("Basement Living Room"));      break;
	case EPlayerRoomState::WINECELLAR: TempText = FText::FromString(TEXT("Wine Cellar"));      break;
	case EPlayerRoomState::NURSERY:   TempText = FText::FromString(TEXT("Nursery"));      break;
	default:
		break;
	}
	if (EnterWidget) {
		if (EnterWidget->LifeTime > 0) {
			EnterWidget->SetText(TempText);
			EnterWidget->LifeTime = 2.0f;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("gdgd"));
			EnterWidget = CreateWidget<UHG_EnterInstruction>(GetWorld(), WidgetFactory);
			EnterWidget->SetText(TempText);
			EnterWidget->AddToViewport();
		}
	}
	LocState = Loc;
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







