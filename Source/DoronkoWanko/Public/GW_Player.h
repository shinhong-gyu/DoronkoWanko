// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "HJ_TrainWheel.h"
#include "helmet.h"
#include "DynamicObject.h"
#include "HJ_MinimapUI.h"
#include "HJMiniMapWidget.h"
#include "DecalInfoStruct.h"
#include "GW_Player.generated.h"

UENUM(BlueprintType)
enum class EPlayerRoomState : uint8
{
	KITCHEN,
	LIVINGROOM,
	BASEMENTLIVINGROOM,
	WINECELLAR,
	NURSERY,
};


UCLASS()
class DORONKOWANKO_API AGW_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGW_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float TargetArmLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float MinArmLength = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float MaxArmLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputMappingContext> IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Zoom;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Dash;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Interaction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Drop;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Shake;


	void OnMyActionMove(const FInputActionValue& Value);

	void OnMyActionLook(const FInputActionValue& Value);

	void OnMyActionJump(const FInputActionValue& Value);

	void OnMyActionZoom(const FInputActionValue& Value);

	void OnMyActionDashOngoing(const FInputActionValue& Value);

	void OnMyActionDashCompleted(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> SplatterFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DashSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WalkSpeed = 500.0f;

public:

	UPROPERTY()
	TObjectPtr<AActor> LookAtActor;

	TArray<FLinearColor> ColorArray;

	int32 CurIdx = 0;
	float IdxSetTime = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> IA_Dirt;

	void OnMyActionDirtStart(const FInputActionValue& Value);

	void OnMyActionDirtOngoing(const FInputActionValue& Value);

	void OnMyActionDirtEnd(const FInputActionValue& Value);

	void OnMyActionShakeEnd(const FInputActionValue& Value);

	void OnMyActionShake(const FInputActionValue& Value);

	// 몸 털기가 지속되고 있을 때 호출될 함수
	void OnMyActionShakeOngoing(const FInputActionValue& Value);

	// 몸 털기 및 물방울 생성 함수
	void ShakeOffAndSpawnSplatter();


	void OnMyActionInteraction(const FInputActionValue& Value);


	void OnMyActionDrop(const FInputActionValue& Value);

	void AttachStaticicObject(AActor* ObjectToAttach);

private:
	// 몸 흔들기
	void Shake();

	UPROPERTY()
	TObjectPtr<AActor> AttachedMasterItem;

	UPROPERTY()
	TObjectPtr <AActor> AttachedStaticObject;

	UPROPERTY()
	TObjectPtr < AActor> OverlappingObject;

	// 계속 물방울을 스폰하지 않도록 지연을 주기위한 변수
	float ShakeDelay = 0.0f;


	void DropObject(AActor* ObjectToDrop);

	void HandleMasterItemAttachment(AActor* ObjectToAttach);

	void HandleStaticObjectAttachment(AActor* ObjectToAttach);

	// 몸이 더럽혀진 정도를 나타내는 변수 0 ~ 100
	float DirtPercentage;

	// 충돌 함수
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// AnimInstance
	UPROPERTY()
	TObjectPtr<UPlayerAnimInstance> Anim;

public:
	bool bIsRightMouseDown;

	// 물건을 물 때의 효과음
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> Bite;

	// 물건을 내려놓을 때의 효과음
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> Drop;

	// 방에 들어갈 때 방의 이름과 찾은 스탬프 개수를 나타내는 UI 
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UHG_EnterInstruction> EnterInstructionUI;

	// 방에 들어갈 때 방의 이름과 찾은 스탬프 개수를 나타내는 UI를 생성하는 클래스 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHG_EnterInstruction> EnterInstructionUIClass;

	// 플레이어가 현재 위치한 방 정보 열거형 변수
	UPROPERTY(EditAnywhere)
	EPlayerRoomState PlayerRoomState;

	// 방이 바뀔 때 마다 LocState를 업데이트 하는 함수
	void SetLocState(EPlayerRoomState Loc);

	// 미니맵 UI를 생성하기 위한 서브 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MinimapUIClass;

	// 미니맵 UI를 저장하기 위한 변수
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UHJMiniMapWidget> MinimapUI;

	// Rub 중일 때 플레이어의 아래에 데칼이 있는지 체크하는 함수
	FDecalInfo* IsDecalInRange(FVector Pos, float DetectRadius);

	// Rub 중에 데칼에 맞았는지 체크하기 위한 플래그 변수
	uint8 bHitDecal : 1;

	// 몇 번째 실행인지 체크하기 위한 변수
	int32 Count = 0;

	// 현재의 색을 나타내는 변수
	FLinearColor RecentColor;

	// Rub 중인지 체크하기 위한 플래그 변수
	uint8 bRubbing : 1;

	// Shake 중인지 체크하기 위한 플래그 변수
	uint8 bShaking : 1;
};
