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
#include "DoronkoGameMode.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "HJMiniMapWidget.h"

// Sets default values
AGW_Player::AGW_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetArmLength = 300.0f;
	ZoomSpeed = 75.0f;
	MinArmLength = 50.0f;
	MaxArmLength = 1000.0f;
	DirtPercentage = 20.0f;

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

	ColorArray.SetNum(1);
	ColorArray[0] = FLinearColor(0.156f, 0.825f, 0.114f);
}

// Called when the game starts or when spawned
void AGW_Player::BeginPlay()
{
	Super::BeginPlay();

	MinimapUI = Cast<UHJMiniMapWidget>(CreateWidget(GetWorld(), MinimapUIClass));
	if(MinimapUI)	MinimapUI->AddToViewport();

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
		UE_LOG(LogTemp, Warning, TEXT("ANim"))
	}

	RecentColor = ColorArray[0];

	UMaterialInterface* CurMaterial = this->GetMesh()->GetMaterial(0);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurMaterial, this);
	if (DynamicMaterial) {
		DynamicMaterial->SetVectorParameterValue("Color", RecentColor);
		this->GetMesh()->SetMaterial(0, DynamicMaterial);
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
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 150.0f, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
	if (bHit) {
		// 바라본 곳에 뭔가 있다.
		if (LookAtActor == nullptr) {
			if (OutHit.GetActor() != LookAtActor) {
				LookAtActor = OutHit.GetActor();
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
	auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	IdxSetTime += DeltaTime;

	if (IdxSetTime > 0.5) {
		for (auto c : ColorArray) {
			UE_LOG(LogTemp, Warning, TEXT("ColorArray : %s"), *c.ToString());
		}
		IdxSetTime = 0;
		if (ColorArray.Num() == 2) {
			if (CurIdx == 0 && ColorArray[1] != FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)) CurIdx = 1;
			else if (CurIdx == 1) CurIdx = 0;
		}
	}

}


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
		Splatter->SetMyColor(ColorArray[CurIdx]);
	}


}
void AGW_Player::OnMyActionDirtStart(const FInputActionValue& Value)
{
	check(Anim)
		if (Anim)
		{
			Anim->PlayRubMontage();
			UE_LOG(LogTemp, Warning, TEXT("Rub"));
		}

	FHitResult hitInfo;
	FCollisionQueryParams params;
	FCollisionObjectQueryParams QParams;

	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	params.AddIgnoredActor(this);

	FDecalInfo* HittedDecalInfo;

	FVector end = GetActorLocation();
	end.Z = -100;
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, GetActorLocation(), end, ECC_Visibility, params);
	if (bHit) {
		HittedDecalInfo = IsDecalInRange(hitInfo.ImpactPoint, 1000.0f, 1000.0f);
		UE_LOG(LogTemp, Warning, TEXT("bHit : %s"), *hitInfo.GetActor()->GetName());
		if (HittedDecalInfo != nullptr) {
			bHitDecal = true;
			RecentColor = HittedDecalInfo->Color;
			UE_LOG(LogTemp, Warning, TEXT("HittedDecal"));
			UE_LOG(LogTemp, Warning, TEXT("Decal Color : %s"), *(HittedDecalInfo->Color.ToString()));
			if (HittedDecalInfo->Color != ColorArray[0] && count == 0) {
				ColorArray.SetNum(2);
			}
			if (ColorArray.Num() == 2) {
				if (count % 2 == 0) {
					ColorArray[0] = HittedDecalInfo->Color;
				}
				if (count == 0) {
					ColorArray[1] = ColorArray[0];
				}
				else if (count % 2 == 1) {
					ColorArray[1] = HittedDecalInfo->Color;
				}
				count++;
			}
		}
		else {
			bHitDecal = false;
		}
	}
}

void AGW_Player::OnMyActionDirtOngoing(const FInputActionValue& Value)
{
	FString MaterialPath;
	UMaterialInterface* NewMaterial;
	if (DirtPercentage < 100.0f && bHitDecal)
	{
		DirtPercentage += 1.0f;
		if (DirtPercentage > 100.0f)
		{
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 5, 5);
			DirtPercentage = 100.0f;
		}
		else if (DirtPercentage >= 80.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 4, 4);
		}
		else if (DirtPercentage >= 60.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 3, 3);
		}
		else if (DirtPercentage >= 40.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 2, 2);
		}
		else if (DirtPercentage >= 20.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 1, 1);
		}
		else {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Dog_Spitz/Spitz/Materials/M_Spitz_color_1.M_Spitz_color_1'"));
		}
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

		UMaterialInterface* CurMaterial = NewMaterial;
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurMaterial, this);
		if (DynamicMaterial) {
			DynamicMaterial->SetVectorParameterValue("Color", RecentColor);
			this->GetMesh()->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AGW_Player::OnMyActionDirtEnd(const FInputActionValue& Value)
{
}


void AGW_Player::OnMyActionSplash(const FInputActionValue& Value)
{
	check(Anim)
		if (Anim)
		{
			Anim->PlaySplashMontage();
		}
	FString MaterialPath;
	UMaterialInterface* NewMaterial;
	if (DirtPercentage != 0.0f)
	{
		DirtPercentage -= 6.0f;
		if (DirtPercentage > 100.0f)
		{
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 5, 5);
			DirtPercentage = 100.0f;
		}
		else if (DirtPercentage >= 80.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 4, 4);
		}
		else if (DirtPercentage >= 60.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 3, 3);
		}
		else if (DirtPercentage >= 40.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 2, 2);
		}
		else if (DirtPercentage >= 20.0f) {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin'"), 1, 1);
		}
		else {
			MaterialPath = FString::Printf(TEXT("/Script/Engine.Material'/Game/Dog_Spitz/Spitz/Materials/M_Spitz_color_1.M_Spitz_color_1'"));
		}
		if (DirtPercentage < 0.0f)
		{
			DirtPercentage = 0.0f;
		}
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

		UMaterialInterface* CurMaterial = NewMaterial;
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurMaterial, this);
		if (DynamicMaterial) {
			DynamicMaterial->SetVectorParameterValue("Color", RecentColor);
			this->GetMesh()->SetMaterial(0, DynamicMaterial);
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
	}
	else if (AStaticObject* StaticObject = Cast<AStaticObject>(OtherActor))
	{
		OverlappingObject = StaticObject;
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
			EnterWidget = CreateWidget<UHG_EnterInstruction>(GetWorld(), WidgetFactory);
			EnterWidget->SetText(TempText);
			EnterWidget->AddToViewport();
		}
	}
	LocState = Loc;
}

FDecalInfo* AGW_Player::IsDecalInRange(FVector Pos, float Param1, float Param2)
{
	FVector Dist;
	FVector Min = FVector(1000.0f, 1000.0f, 1000.0f);
	int32 RetIdx = -1;
	ADoronkoGameMode* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		for (int i = 0; i < GM->SpawnedDecalArr.Num(); i++) {
			Dist = Pos - GM->SpawnedDecalArr[i].DecalComp->GetComponentLocation();
			// 만약 원하는 거리 안에 있으면
			if (Param1 * Param1 + Param2 * Param2 > Dist.Size() * Dist.Size()) {
				// 가장 가까운 DecalComp찾기
				// Min 값 업데이트
				if (Min.Size() >= Dist.Size()) {
					Min = Dist;
					RetIdx = i;
				}
			}
		}
		if (RetIdx > 0 && RetIdx < GM->SpawnedDecalArr.Num())
		{
			return &(GM->SpawnedDecalArr[RetIdx]);
		}
		else
			return nullptr;//FDecalInfo();
	}
	else return nullptr;
}








