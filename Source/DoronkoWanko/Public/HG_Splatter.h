// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalInfoStruct.h"
#include "HG_Splatter.generated.h"

UCLASS()
class DORONKOWANKO_API AHG_Splatter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHG_Splatter();

private:
	

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
    TSubclassOf<class AHG_DecalActor> DecalClass;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	FVector ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal);

	FVector Velocity;

	UPROPERTY(EditAnywhere)
	class UMaterial* SelectedMaterial;

 	UFUNCTION()
 	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Initalize(FVector initVeloccity);

	void UpdataRotation();

	TArray<class AHG_MissonStamp*> IsStampInRange(FVector Pos ,float Param1,float Param2);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AHG_MissonStamp> StampFactory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> NormalArrow;
	
	FLinearColor MyColor = FLinearColor();

	void SetMyColor(FLinearColor Value);

	int32 SO = 0;
	FDecalInfo* IsDecalInRange(FVector Pos,float Param1,float Param2);

	bool bSpawnedByRV = false;
};
