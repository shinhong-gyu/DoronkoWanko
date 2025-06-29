// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HJ_Splatter2.generated.h"

UCLASS()
class DORONKOWANKO_API AHJ_Splatter2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHJ_Splatter2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	FVector Velocity;

	UPROPERTY(EditAnywhere)
	TObjectPtr <class UMaterial> SelectedDecalMaterial;

	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal);

	void Initalize(FVector initVeloccity);

	void UpdataRotation();

	void SetMyColor(FLinearColor Value);

	TArray<TObjectPtr<class AHG_MissonStamp>> IsStampInRange(FVector Pos, float Param1, float Param2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AHG_MissonStamp> StampClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> NormalArrow;

	FLinearColor MyColor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> StampSFX;

	int32 SortOrder = 0;
};
