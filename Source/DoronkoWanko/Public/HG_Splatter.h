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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌 함수
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 물감이 스폰됐을 때 속력을 초기화하는 함수
	void Initalize(FVector initVeloccity);

	// 충돌체
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

	// 물방울 매시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	// Splatter Section	
public:
	// 물방울의 색을 설정하는 함수
	void SetMyColor(FLinearColor Value);

	FORCEINLINE FLinearColor GetMyColor() const { return MyColor; }

	//  bSpawnedByRV Getter/Setter
	FORCEINLINE void SetSpawnedByRV(bool Value) { bSpawnedByRV = Value; }

	FORCEINLINE bool GetSpawnedByRV() const { return bSpawnedByRV; }


private:

	// 물방울 색
	FLinearColor MyColor = FLinearColor();

	// 로봇 청소기에 의해 스폰되었는지 여부
	bool bSpawnedByRV = false;

	// Decal Section
public:
	// 데칼이 생성될 방향을 결정하기 위한 함수 (정사영)
	FVector ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal);

	// 물방울이 충돌했을 때 범위 안에 데칼이 있는지 확인하고 해당 데칼의 정보를 반환하는 함수.
	FDecalInfo* IsDecalInRange(FVector Pos, float InRange);

private:
	// 데칼 생성을 위한 TSubClassOf (팩토리 패턴)
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AHG_DecalActor> DecalClass;

	// 데칼이 스폰될 때 사용할 머터리얼 (물감 모양)
	UPROPERTY(EditAnywhere)
	class UMaterial* SelectedDecalMaterial;

	// 데칼이 생성될 때 SortOrder 값을 설정하기 위한 변수
	int32 SortOrder = 0;

	// Physics Section
public:
	// 물감이 Velocity 방향으로 날아가며 회전하도록 하는 함수
	void UpdataRotation();

private:
	// 속도
	FVector Velocity;

	// Stamp Section
public:
	// 물방울이 충돌한 위치에서 일정 범위 내에 스팸프가 있는지 확인하고 범위 내에 있는 스팸프들을 반환하는 함수.
	TArray<class AHG_MissonStamp*> IsStampInRange(FVector Pos, float InRange);

	// 스팸프를 찾기 위한 스팸프 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AHG_MissonStamp> StampClass;

	// 찾은 스탬프들을 저장하기 위한 배열
	TArray<AActor*> StampArray;
};
