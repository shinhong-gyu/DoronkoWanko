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
	bRubbing = false;
	bShaking = false;
	bHitDecal = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = TargetArmLength;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));

	AttachedMasterItem = nullptr;
	AttachedStaticObject = nullptr;
	OverlappingObject = nullptr;

	ColorArray.SetNum(1);
	ColorArray[0] = FLinearColor(0.156f, 0.825f, 0.114f);

	PlayerRoomState = EPlayerRoomState::LIVINGROOM;
}

// Called when the game starts or when spawned
void AGW_Player::BeginPlay()
{
	Super::BeginPlay();

	// 미니맵 UI 생성
	MinimapUI = Cast<UHJMiniMapWidget>(CreateWidget(GetWorld(), MinimapUIClass));

	// 미니맵 UI가 생성에 성공했다면
	if (MinimapUI)
	{
		// 화면에 추가하고
		MinimapUI->AddToViewport();

		// 1층 미니맵으로 초기화
		MinimapUI->ShowFloor(1);
	}

	// EnhancedInput
	APlayerController* PC = Cast<APlayerController>(Controller);

	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (SubSys)
		{
			SubSys->AddMappingContext(IMC_Player, 0);
		}

		PC->SetInputMode(FInputModeGameOnly());
	}

	// 애님 인스턴스 저장
	Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// 플레이어의 0번 머터리얼을 저장
	UMaterialInterface* CurMaterial = GetMesh()->GetMaterial(0);

	// 플레이어의 0번 머터리얼을 다이나믹 머터리얼로 생성
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurMaterial, this);

	// 생성에 성공했다면
	if (DynamicMaterial)
	{
		// ColorArray에 있는 0번째 인덱스를 몸의 색으로 설정
		CurrentColor = ColorArray[0];

		// DynamicMaterial의 Color 파라미터를 RecentColor로 설정
		DynamicMaterial->SetVectorParameterValue("Color", CurrentColor);

		// DynamicMaterial을 0번 머터리얼로 설정
		GetMesh()->SetMaterial(0, DynamicMaterial);
	}
}

// Called every frame
void AGW_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 움직임
	FTransform Transfrom = FTransform(GetControlRotation());
	Direction = Transfrom.TransformVector(Direction);
	Direction.Z = 0;
	Direction.Normalize();
	AddMovementInput(Direction, 1);
	Direction = FVector::ZeroVector;

	// @Author : 신홍규
	// LineTrace 파라미터 선언
	FHitResult OutHit;
	FVector Start = GetActorLocation() - FVector(0, 0, -36.666f);
	FVector End = Start + GetActorForwardVector() * 100;
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// LineTrace
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 150.0f, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

	// LineTrace에 맞은게 있다면
	if (bHit)
	{
		// LookAtActor가 Null이라면 (바라보고 있던 액터가 없다면)
		if (LookAtActor == nullptr)
		{
			// LineTrace에 맞은 액터를 LookAtActor로 설정
			LookAtActor = OutHit.GetActor();

			// 바라보고 있는 액터가 Interaction 인터페이스를 구현했는지 확인
			II_Interaction* Interactable = Cast<II_Interaction>(LookAtActor);

			// 구현했다면
			if (nullptr != Interactable)
			{
				// 바라볼 때의 동작 실행
				Interactable->LookAt();
			}
			// 아니라면
			else
			{
				// LookAtActor를 비운다
				LookAtActor = nullptr;
			}

		}
	}
	// 아무것도 맞지 않았다면
	else
	{
		// 만약 이전 Tick에서 LookAtActor가 있었다면
		if (nullptr != LookAtActor)
		{
			// LookAtActor가 Interaction 인터페이스를 구현했는지 확인
			II_Interaction* Interactable = Cast<II_Interaction>(LookAtActor);

			// 구현했다면
			if (Interactable)
			{
				// 오브젝트를 바라보다 다른 곳을 바라볼 때의 동작 실행
				Interactable->FadeAway();

				// LookAtActor를 비운다.
				LookAtActor = nullptr;
			}
		}
	}

	// TargetArmLength를 줌 기능으로 조작했다면 변경된 TargetArmLength까지 보간
	SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetArmLength, DeltaTime, ZoomSpeed);

	// @Author : 신홍규
	// CurIdx를 지속적으로 바꿔주도록 하는 로직
	IdxSetTime += DeltaTime;

	// 일정시간이 지나면
	if (IdxSetTime > 0.5)
	{
		// IdxSetTime를 0으로 초기화
		IdxSetTime = 0;

		// 만약 ColorArray에 2개의 색이 들어있는 상태라면
		if (ColorArray.Num() == 2)
		{
			// CurIdx를 번갈아가면서 Shake할때의 물방울 색 결정

			if (CurIdx == 0 && ColorArray[1] != FLinearColor(0.0f, 0.0f, 0.0f, 0.0f))
			{
				CurIdx = 1;
			}

			else if (CurIdx == 1)
			{
				CurIdx = 0;
			}
		}
	}

	// @Author : 신홍규
	// 흔드는 중이라면 일정 딜레이를 주어 물방울을 스폰하도록 제어
	if (bShaking)
	{
		ShakeDelay += DeltaTime;
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

		input->BindAction(IA_Shake, ETriggerEvent::Started, this, &AGW_Player::OnMyActionShake);
		input->BindAction(IA_Shake, ETriggerEvent::Triggered, this, &AGW_Player::OnMyActionShakeOngoing);
		input->BindAction(IA_Shake, ETriggerEvent::Completed, this, &AGW_Player::OnMyActionShakeEnd);

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

void AGW_Player::OnMyActionDashOngoing(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
}

void AGW_Player::OnMyActionDashCompleted(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// @Author : 신홍규
void AGW_Player::Shake()
{
	// 랜덤으로 물방울의 초기 속도 생성
	FVector InitialVelocity = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(300, 600));

	// 스폰 트랜스폼
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 물방울 액터(AHG_Splatter) 생성
	AHG_Splatter* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, SpawnLocation, SpawnRotation);

	// 생성에 성공했다면
	if (Splatter)
	{
		// 속도를 초기화하고
		Splatter->Initalize(InitialVelocity);

		// 색깔도 설정
		// 만약 현재 인덱스가 유효하지 않은 색이 아니라면
		if (ColorArray[CurIdx] != FLinearColor(0.0f, 0.0f, -0.000009f, 0.0f))
		{
			// 현재 인덱스로 물방울 색 설정
			Splatter->SetMyColor(ColorArray[CurIdx]);
		}
		// 유효하지 않다면
		else
		{
			// 만약 ColorArray에 2개의 색이 들어있는 상태라면
			if (ColorArray.Num() == 2)
			{
				// 다른 인덱스의 물방울 색 설정
				Splatter->SetMyColor(ColorArray[CurIdx == 0 ? 1 : 0]);
			}
		}
	}
}

// @Author : 신홍규
void AGW_Player::OnMyActionDirtStart(const FInputActionValue& Value)
{
	// Anim이 유효한지 체크
	ensure(Anim);

	// Rub 플래그를 true로 변경
	bRubbing = true;

	// Rub 애니메이션 몽타주 재생
	if (Anim)
	{
		Anim->PlayRubMontage();
	}


	// LineTrace를 사용하여 바닥에 있는 데칼을 찾기 위한 파라미터 설정
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation();
	// z방향으로 1m만큼 라인트레이스를 발사하도록 파라미터 설정
	End.Z = -100.0f;

	FHitResult HitInfo;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams QParams;
	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	Params.AddIgnoredActor(this);

	// LineTrace의 충돌 지점 근처에 있는 데칼을 저장하기 위한 변수
	FDecalInfo* HittedDecalInfo;


	// LineTrace를 사용하여 바닥에 있는 데칼을 찾기
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, Start, End, ECC_Visibility, Params);

	// LineTrace가 성공적으로 충돌한 경우
	if (bHit)
	{
		// 충돌 지점 근처에 있는 데칼 정보를 가져오기
		HittedDecalInfo = IsDecalInRange(HitInfo.ImpactPoint, 1000.0f);

		// 라인트레이스의 충돌 지점 근처에 데칼이 있었다면
		if (HittedDecalInfo != nullptr)
		{
			// 플래그 변경
			bHitDecal = true;

			// 라인트레이스에 맞은 데칼의 색을 현재의 색으로 설정
			CurrentColor = HittedDecalInfo->Color;

			// HittedDecalInfo 의 색이 현재 색과 다르면서 Count가 0이라면(첫 실행이라면)
			if (HittedDecalInfo->Color != ColorArray[0] && Count == 0)
			{
				// ColorArray의 크기를 2로 설정
				ColorArray.SetNum(2);
			}

			// ColorArray에 2개의 색이 채워진 상태라면
			if (ColorArray.Num() == 2)
			{
				// 첫 실행이라면
				if (Count == 0)
				{
					ColorArray[1] = ColorArray[0];
				}

				// Count가 짝수일 땐
				if (Count % 2 == 0)
				{
					// ColorArray 0번 인덱스를 데칼의 색으로 만들고
					ColorArray[0] = HittedDecalInfo->Color;
				}
				else if (Count % 2 == 1)
				{
					ColorArray[1] = HittedDecalInfo->Color;
				}


				Count++;
			}
		}
		// 없었다면
		else
		{
			// 플래그를 false로 변경
			bHitDecal = false;
		}
	}
}

// @Author : 신홍규
void AGW_Player::OnMyActionDirtOngoing(const FInputActionValue& Value)
{
	// 머터리얼의 경로를 저장할 변수
	FString MaterialPath;

	// 머터리얼을 로드해서 저장할 변수
	UMaterialInterface* NewMaterial;

	// 만약 Rub를 실행했는데 아래쪽에 데칼이 있었다면
	if (bHitDecal)
	{
		// 1번의 호출 당 0.5%씩 증가 DirtPercentage 0~100 사이 값으로 조정
		DirtPercentage = FMath::Clamp(DirtPercentage + 0.5f, 0.0f, 100.0f);

		// DirtPercentage 에 따른 플레이어 메시 머터리얼 변화 (20%마다 변화)
		int assetNum = DirtPercentage / 20;

		// 경로 지정
		MaterialPath = FString::Printf(TEXT("/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin"), assetNum, assetNum);

		// 지정된 경로명을 통해 머터리얼 로드
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

		// 로드에 성공했다면
		if (NewMaterial)
		{
			// NewMaterial를 다이나믹 머터리얼로 만들고
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(NewMaterial, this);

			// 다이나믹 머터리얼을 생성하는데 성공했다면
			if (DynamicMaterial)
			{
				// ColorArray의 0번 인덱스를 RecentColor로 설정하고
				//CurrentColor = ColorArray[0];

				// 다이나믹 머터리얼의 Color 파라미터를 RecentColor로 설정
				DynamicMaterial->SetVectorParameterValue("Color", CurrentColor);

				// DynamicMaterial을 플레이어 메시의 0번 머터리얼로 설정
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
		}

	}
}

void AGW_Player::OnMyActionDirtEnd(const FInputActionValue& Value)
{
	bRubbing = false;
}

void AGW_Player::OnMyActionShakeEnd(const FInputActionValue& Value)
{
	bShaking = false;
	ShakeDelay = 0.0f;
}

// @Author : 신홍규
void AGW_Player::OnMyActionShake(const FInputActionValue& Value)
{
	// 플래그 변경
	bShaking = true;

	ensure(Anim);

	// Anim이 있을 때만 애니메이션 재생
	if (Anim)
	{
		Anim->PlaySplashMontage();
	}

	// 몸 털기 및 물방울 생성 함수 호출
	ShakeOffAndSpawnSplatter();
}


// @Author : 신홍규
void AGW_Player::OnMyActionShakeOngoing(const FInputActionValue& Value)
{
	// ShakeDelay가 0.5초 이하라면 아무것도 하지않음
	if (ShakeDelay < 0.5f)
	{
		return;
	}

	// 플래그 변경
	bShaking = true;

	// 몸 털기 및 물방울 생성 함수 호출
	ShakeOffAndSpawnSplatter();

	// 모든 Shake로직이 끝나면 ShakeDelay 초기화
	ShakeDelay = 0.0f;
}

void AGW_Player::ShakeOffAndSpawnSplatter()
{
	// 머터리얼의 경로를 저장할 변수
	FString MaterialPath;

	// 머터리얼을 로드해서 저장할 변수
	UMaterialInterface* NewMaterial;

	// DirtPercentage 가 0이 아니라면
	if (DirtPercentage != 0.0f)
	{
		// 한 번의 호출 당 6.0씩 감소, 0 이하로 떨어지지 않도록 Clamp
		DirtPercentage = FMath::Clamp(DirtPercentage - 6.0f, 0, 100);

		// DirtPercentage 에 따른 플레이어 메시 머터리얼 변화
		int assetNum = DirtPercentage / 20;

		// 경로 지정
		MaterialPath = FString::Printf(TEXT("/Game/Material/BaseMaterials/M_Spitz_%d_Origin.M_Spitz_%d_Origin"), assetNum, assetNum);

		// 지정된 경로명을 통해 머터리얼 로드
		NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

		// 로드에 성공했다면
		if (NewMaterial)
		{
			// NewMaterial를 다이나믹 머터리얼로 만들고
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(NewMaterial, this);

			// 다이나믹 머터리얼을 생성하는데 성공했다면
			if (DynamicMaterial)
			{
				// ColorArray의 0번 인덱스를 RecentColor로 설정하고
				CurrentColor = ColorArray[0];

				// 다이나믹 머터리얼의 Color 파라미터를 RecentColor로 설정
				DynamicMaterial->SetVectorParameterValue("Color", CurrentColor);

				// DynamicMaterial을 플레이어 메시의 0번 머터리얼로 설정
				GetMesh()->SetMaterial(0, DynamicMaterial);
			}
		}

		// 물방울을 몇 개 생성할지 랜덤으로 결정
		int NumberOfSplatter = FMath::RandRange(3, 5);

		// NumberOfSplatter번 반복
		for (int i = 0; i < NumberOfSplatter; i++)
		{
			// 물방울 생성
			Shake();
		}

	}
}

void AGW_Player::OnMyActionInteraction(const FInputActionValue& Value)
{
	if (LookAtActor != nullptr)
	{
		II_Interaction* Interactable = Cast<II_Interaction>(LookAtActor);
		if (Interactable != nullptr)
		{
			Interactable->InteractionWith();

			if (AMasterItem* MasterItem = Cast<AMasterItem>(LookAtActor))
			{
				HandleMasterItemAttachment(MasterItem);
			}
			else if (AStaticObject* StaticObject = Cast<AStaticObject>(LookAtActor))
			{
				HandleStaticObjectAttachment(StaticObject);
				UGameplayStatics::PlaySound2D(GetWorld(), Bite);
			}
		}

		if (Interactable)
		{
			Interactable->FadeAway();
		}
	}
	LookAtActor = nullptr;
}

void AGW_Player::OnMyActionDrop(const FInputActionValue& Value)
{
	if (AttachedStaticObject != nullptr)
	{
		II_Interaction* Interact = Cast<II_Interaction>(AttachedStaticObject);

		if (Interact != nullptr)
		{
			Interact->ItemDrop();
		}

		DropObject(AttachedStaticObject);
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

		DropObject(AttachedMasterItem);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), Drop);
}

void AGW_Player::AttachStaticicObject(AActor* ObjectToAttach)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);


	if (AMasterItem* MasterItem = Cast<AMasterItem>(ObjectToAttach))
	{
		// Attach MasterItem to "HAT" socket
		MasterItem->AttachToComponent(GetMesh(), AttachmentRules, FName("HAT"));

		MasterItem->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X, 1.0f / GetMesh()->GetComponentScale().Y, 1.0f / GetMesh()->GetComponentScale().Z));

		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(MasterItem->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		AttachedMasterItem = MasterItem;
	}

	else if (AStaticObject* StaticicObject = Cast<AStaticObject>(ObjectToAttach))
	{
		// Attach DynamicObject to "HAND" socket
		StaticicObject->AttachToComponent(GetMesh(), AttachmentRules, FName("attach"));

		StaticicObject->SetActorRelativeScale3D(FVector(1.0f / GetMesh()->GetComponentScale().X, 1.0f / GetMesh()->GetComponentScale().Y, 1.0f / GetMesh()->GetComponentScale().Z));
		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(StaticicObject->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		AttachedStaticObject = StaticicObject;

	}
}


void AGW_Player::DropObject(AActor* ObjectToDrop)
{

	if (ObjectToDrop)
	{
		ObjectToDrop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		if (UPrimitiveComponent* DObComp = Cast<UPrimitiveComponent>(ObjectToDrop->GetRootComponent()))
		{
			DObComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			DObComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
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
	if (AttachedMasterItem != nullptr)
	{
		II_Interaction* Interact = Cast<II_Interaction>(AttachedMasterItem);
		if (Interact != nullptr)
		{
			Interact->ItemDrop();
		}
		DropObject(AttachedMasterItem);
	}
	// Attach new MasterItem
	AttachStaticicObject(ObjectToAttach);

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
		DropObject(AttachedStaticObject);
	}

	// Attach new DynamicObject
	AttachStaticicObject(ObjectToAttach);

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
	if (PlayerRoomState == Loc) return;

	// EnterWidget에 방정보 위젯에 설정할 텍스트
	FText TempText;

	// 방 정보 Enum에 따라 텍스트 설정
	switch (Loc)
	{
	case EPlayerRoomState::KITCHEN:   TempText = FText::FromString(TEXT("Kitchen"));   break;
	case EPlayerRoomState::LIVINGROOM:   TempText = FText::FromString(TEXT("Living Room"));      break;
	case EPlayerRoomState::BASEMENTLIVINGROOM:   TempText = FText::FromString(TEXT("Basement Living Room"));      break;
	case EPlayerRoomState::WINECELLAR: TempText = FText::FromString(TEXT("Wine Cellar"));      break;
	case EPlayerRoomState::NURSERY:   TempText = FText::FromString(TEXT("Nursery"));      break;
	default: break;
	}

	// 만약 EnterWidget이 nullptr이 아니라면
	if (EnterInstructionUI)
	{
		// EnterWidget의 생명주기가 0보다 크다면
		if (EnterInstructionUI->LifeTime > 0)
		{
			// EnterWidget의 방이름을 TempText로 설정하고
			EnterInstructionUI->SetRoomName(TempText);

			// 생명 주기를 2초로 초기화한다.
			EnterInstructionUI->LifeTime = 2.0f;
		}
	}
	// nullptr이라면
	else
	{
		// EnterWidget을 생성하고
		EnterInstructionUI = CreateWidget<UHG_EnterInstruction>(GetWorld(), EnterInstructionUIClass);

		// EnterWidget의 방이름을 설정하고
		EnterInstructionUI->SetRoomName(TempText);

		// 생명 주기를 2초로 설정한다.
		EnterInstructionUI->AddToViewport();
	}

	// Loc을 현재 플레이어의 방 상태로 설정한다.
	PlayerRoomState = Loc;
}

FDecalInfo* AGW_Player::IsDecalInRange(FVector Pos, float DetectRadius)
{
	// 거리 벡터
	FVector Dist;

	// 가장 가까운 데칼을 찾기 위한 변수
	FVector Min = FVector(1000.0f, 1000.0f, 1000.0f);

	// 반환할 데칼 인덱스
	int32 ReturnIndex = -1;

	// GameMode 가져오기
	ADoronkoGameMode* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	// GameMode가 존재한다면
	if (GM)
	{
		// GameMode의 SpawnedDecalArr를 순회하면서
		for (int i = 0; i < GM->SpawnedDecalArr.Num(); i++)
		{
			// Pos 에서 데칼으로의 벡터 계산
			Dist = Pos - GM->SpawnedDecalArr[i].DecalComp->GetComponentLocation();

			// 만약 계산한 Dist 를 반지름으로 하는 원이 DetectRadius 를 반지름으로하는 원 안에 있다면 (원하는 거리 안에 있다면)
			if (DetectRadius > Dist.Size())
			{
				// 만약 현재 저장되어있는 최소 거리보다 현재 계산한 거리가 더 작다면
				if (Min.Size() >= Dist.Size())
				{
					// 최소 거리 값을 갱신하고
					Min = Dist;

					// 반환 인덱스를 최소 거리에 있는 데칼의 인덱스로 업데이트
					ReturnIndex = i;
				}
			}
		}

		// 만약 찾은 데칼의 인덱스가 0보다 크고 SpawnedDecalArr의 개수보다 작다면
		if (ReturnIndex > 0 && ReturnIndex < GM->SpawnedDecalArr.Num())
		{
			// 찾은 데칼의 정보를 반환
			return &(GM->SpawnedDecalArr[ReturnIndex]);
		}
	}
	// GM을 가져오는데 실패하거나 찾은 데칼이 없다면
	return nullptr;

}








