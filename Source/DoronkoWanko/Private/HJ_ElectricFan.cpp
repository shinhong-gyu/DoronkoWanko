// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "Components/BoxComponent.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InputComp = nullptr;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	UE_LOG(LogTemp, Warning, TEXT("E key Presse"));
}

// Called when the game starts or when spawned
void AHJ_ElectricFan::BeginPlay()
{
	Super::BeginPlay();
	

	// 특정조건이 만족되면 스폰한다 (다른 객체에서 진행) 
}

// Called every frame
void AHJ_ElectricFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// E키를 누르면 반응을 시작한다
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			EnableInput(PlayerController);
			if (!InputComp)
			{
				InputComp = NewObject<UInputComponent>(this);
				InputComp->RegisterComponent();
			}

			InputComp->BindAction("Interact", IE_Pressed, this, &AHJ_ElectricFan::Interact);
		}
		 
	// 플레이어가 가까이 오면 인식한다 (공통함수) 
	// 조건을 2가지로 나눈다. A와 B 
}

void AHJ_ElectricFan::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("E key Presse"));
}
// A일 때, 선풍기 날개 돌아간다. 
// A일 때, 투명한 충돌체 소환한다 (선풍기 바람범위) // 충돌체에 따른 물감 이동은 물감 객체에서 구현 

// B일 때, 선풍기 날개 멈춘다. 
// B일 때, 투명한 충돌체 사라진다. 
