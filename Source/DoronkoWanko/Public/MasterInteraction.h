// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "I_Interaction.h"
#include "ObjectWidget.h"
#include "MasterInteraction.generated.h"

UCLASS()
class DORONKOWANKO_API AMasterInteraction : public AActor, public II_Interaction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMasterInteraction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComp;

	// 매시
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	// 인터렉션 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComp;

	// 위젯 생성을 위한 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UObjectWidget> ObjectWidgetClass;

	// 생성한 위젯을 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UObjectWidget> Widget;

	// Player 가 아이템을 바라보고 있을 때
	virtual void LookAt();
	// Player 가 아이템을 보다 다른 곳을 바라볼 때
	virtual void FadeAway();
	// E 가 눌렸을 때 할 행동
	virtual void InteractionWith();
	// Q 가 눌렸을 때 할 행동
	virtual void ItemDrop();

	FText InteractionText;

	bool bIsDropped = true;
};
