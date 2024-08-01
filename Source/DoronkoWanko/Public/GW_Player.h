// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "HJ_TrainWheel.h"
#include "helmet.h"
#include "DynamicObject.h"
#include "GW_Player.generated.h"

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
    float WalkSpeed = 250.0f;


	AActor* LookAtActor = nullptr;
		TArray<FColor> ColorArray;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Dirt;

	void OnMyActionDirt(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Splash;

	void OnMyActionSplash(const FInputActionValue& Value);

		UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interaction;

		UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Drop;

	void OnMyActionInteraction(const FInputActionValue& Value);


	void OnMyActionDrop(const FInputActionValue& Value);
	
	void attachDynamicObject() ;

	class AActor* AttachedDOb;
	class AActor* OverlappingDObject;
	

	bool bCanDropAttachedDOb;


	void dropDynamicObject();

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
