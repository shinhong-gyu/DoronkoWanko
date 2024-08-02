// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteraction.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMasterInteraction::AMasterInteraction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	InteractionText = FText::FromString(TEXT("E) INTERACTION"));
}

// Called when the game starts or when spawned
void AMasterInteraction::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMasterInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterInteraction::LookAt()
{
	// 아이템 속성에 따라 Interaction 문구를 Widget 의 TextBox 에 바인드하고
	// 위젯을 생성하여 화면에 표시하고 싶다.
	if (bIsDropped) // 아이템이 떨어져 있고
	{
		UE_LOG(LogTemp,Warning,TEXT("a"))
		if (Widget == nullptr)  // Widget 이 아직 생성되지 않았다면
		{	
			// Widget 을 생성하고
			Widget = CreateWidget<UObjectWidget>(GetWorld(), ObjectWidgetClass);
			if (Widget != nullptr) //생성하는데 성공 했다면
			{
				Widget->SetText(InteractionText);
				// 화면에 Widget 을 출력
				Widget->AddToViewport();
			}
		}
		else // Widget 이 이미 있다면 
		{
			// 그냥 화면에 Widget 을 출력
			Widget->AddToViewport();
		}
	}
}

void AMasterInteraction::FadeAway()
{
	// 화면에 표시되어 있던 위젯을 지우고 싶다.
	if (bIsDropped) // 아이템이 떨어져 있고
	{
		if (Widget != nullptr)  // Widget 이 유효하다면
		{	
			// 화면에서 제거
			Widget->RemoveFromParent();
		}
	}
}

void AMasterInteraction::InteractionWith()
{
	// 플레이어가 Object 를 바라보며 E 를 눌렀을 때 동작 할 함수
}

void AMasterInteraction::ItemDrop()
{
	// 플레이어의 Object Socket 에 Item 이 있을 때 Q 를 누르면 동작 할 함수
}


