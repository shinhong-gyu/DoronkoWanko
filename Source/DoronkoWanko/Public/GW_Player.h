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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float TargetArmLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float MinArmLength = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraComp", meta = (AllowPrivateAccess = "true"))
	float MaxArmLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;



	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;
	void OnMyActionMove(const FInputActionValue& Value);

	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	void OnMyActionLook(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Jump;

	void OnMyActionJump(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Zoom;
	void OnMyActionZoom(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dash;
	void OnMyActionDashOngoing(const FInputActionValue& Value);
	void OnMyActionDashCompleted(const FInputActionValue& Value);
	void Shake();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_Splatter> SplatterFactory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DashSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WalkSpeed = 500.0f;

public:
	AActor* LookAtActor = nullptr;
	TArray<FLinearColor> ColorArray;

	int32 CurIdx = 0;
	float IdxSetTime = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dirt;

	void OnMyActionDirtStart(const FInputActionValue& Value);
	void OnMyActionDirtOngoing(const FInputActionValue& Value);

	void OnMyActionDirtEnd(const FInputActionValue& Value);


	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Splash;

	void OnMyActionSplash(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interaction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Drop;

	void OnMyActionInteraction(const FInputActionValue& Value);


	void OnMyActionDrop(const FInputActionValue& Value);

	void attachStaticicObject(AActor* ObjectToAttach);

	UPROPERTY()
	class AActor* AttachedMasterItem;

	UPROPERTY()
	class AActor* AttachedStaticObject;

	UPROPERTY()
	class AActor* OverlappingObject;




	void dropObject(AActor* ObjectToDrop);
	void HandleMasterItemAttachment(AActor* ObjectToAttach);
	void HandleStaticObjectAttachment(AActor* ObjectToAttach);

	float DirtPercentage;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float MakeTime = 0.5;
	float CurrentTime = 0;

	//Animation
	UPROPERTY()
	class UPlayerAnimInstance* Anim;

	bool bIsRightMouseDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USoundBase* Bite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USoundBase* Drop;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UHG_EnterInstruction> WidgetFactory;

	UPROPERTY(EditAnywhere)
	class UHG_EnterInstruction* EnterWidget = nullptr;
	// 플레이어가 현재 위치한 방
	UPROPERTY(EditAnywhere)
	EPlayerRoomState LocState = EPlayerRoomState::LIVINGROOM;

	// 방이 바뀔 때 마다 LocState를 업데이트 하는 함수
	void SetLocState(EPlayerRoomState Loc);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MinimapUIClass;

	UHJMiniMapWidget* MinimapUI;

	FDecalInfo* IsDecalInRange(FVector Pos, float Param1, float Param2);

	int32 count = 0;

	bool bHitDecal = false;

	FLinearColor RecentColor;

};
