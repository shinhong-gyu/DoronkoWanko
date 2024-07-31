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
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UWidgetComponent* InteractionWidgetComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UObjectWidget> ObjectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UObjectWidget* Widget;

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
