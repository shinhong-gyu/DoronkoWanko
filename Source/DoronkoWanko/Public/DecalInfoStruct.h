// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/GCObject.h"
#include "DecalInfoStruct.generated.h"

UCLASS()
class DORONKOWANKO_API ADecalInfoStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalInfoStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic,BlueprintType)
struct FDecalInfo // 생성된 데칼의 정보 구조체
{
	GENERATED_BODY()
public:
	// 데칼 컴포넌트
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDecalComponent> DecalComp;

	// 데칼이 스폰된 위치 (위치를 통해 데칼이 커질지 안커질지 혹은 스탬프가 있는 곳에 스폰되는지 판별)
	UPROPERTY(EditAnywhere)
	FVector Pos;

	// 데칼의 색상 (색이 같으면 데칼의 크기가 커짐)
	UPROPERTY(EditAnywhere)
	FLinearColor Color;

	FDecalInfo() :
		DecalComp(nullptr), Pos(FVector::ZeroVector), Color(FLinearColor::White)
	{}
};


