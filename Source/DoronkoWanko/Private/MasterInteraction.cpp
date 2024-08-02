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
	// ������ �Ӽ��� ���� Interaction ������ Widget �� TextBox �� ���ε��ϰ�
	// ������ �����Ͽ� ȭ�鿡 ǥ���ϰ� �ʹ�.
	if (bIsDropped) // �������� ������ �ְ�
	{
		UE_LOG(LogTemp,Warning,TEXT("a"))
		if (Widget == nullptr)  // Widget �� ���� �������� �ʾҴٸ�
		{	
			// Widget �� �����ϰ�
			Widget = CreateWidget<UObjectWidget>(GetWorld(), ObjectWidgetClass);
			if (Widget != nullptr) //�����ϴµ� ���� �ߴٸ�
			{
				Widget->SetText(InteractionText);
				// ȭ�鿡 Widget �� ���
				Widget->AddToViewport();
			}
		}
		else // Widget �� �̹� �ִٸ� 
		{
			// �׳� ȭ�鿡 Widget �� ���
			Widget->AddToViewport();
		}
	}
}

void AMasterInteraction::FadeAway()
{
	// ȭ�鿡 ǥ�õǾ� �ִ� ������ ����� �ʹ�.
	if (bIsDropped) // �������� ������ �ְ�
	{
		if (Widget != nullptr)  // Widget �� ��ȿ�ϴٸ�
		{	
			// ȭ�鿡�� ����
			Widget->RemoveFromParent();
		}
	}
}

void AMasterInteraction::InteractionWith()
{
	// �÷��̾ Object �� �ٶ󺸸� E �� ������ �� ���� �� �Լ�
}

void AMasterInteraction::ItemDrop()
{
	// �÷��̾��� Object Socket �� Item �� ���� �� Q �� ������ ���� �� �Լ�
}


