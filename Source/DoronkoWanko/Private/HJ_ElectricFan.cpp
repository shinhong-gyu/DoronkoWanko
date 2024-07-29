// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_ElectricFan.h"
#include "GW_Player.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_ElectricFan::AHJ_ElectricFan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// 충돌체 생성 
	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//SetRootComponent(BoxComp);
	//BoxComp->SetBoxExtent(FVector(50));
	//// 메쉬 생성 
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(RootComponent);

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_ElectricFan::OnOverlapEnd);

	// 날개 메쉬 생성 
	WingComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingComp"));
	WingComp->SetupAttachment(RootComponent);
	// 회전 기본값 지정 
	RotationSpeed = 200.0f;
	CurrentRotationAngel = 0.0f;

	IsInRange = false;

	InteractionText = FText::FromString(TEXT("E) INTERACTION"));
}

// Called when the game starts or when spawned
void AHJ_ElectricFan::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHJ_ElectricFan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 범위 안에 들어왔을 때 OnAction 함수를 호출 
	if (IsInRange)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
		{
			RotationSpeed = 100.0f;
			PressE = PressE + 1;
		}
	}
	
	if (PressE % 2 == 1)
	{
		// 선풍기 날개 돌아가게 한다
		// E키를 한번 누르면 돌아가고 & 두번 누르면 돌아가는게 멈춘다 
		CurrentRotationAngel += RotationSpeed * DeltaTime;
		if (CurrentRotationAngel > 360.0f)
		{
			CurrentRotationAngel -= 360.0f;
		}

		FRotator WingRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
		WingComp->SetRelativeRotation(WingRotation);

		//충돌체를 소환한다 (물감과 부딪히면 특정 위치로 물감을 보내버린다) 
		// else 일 때, 충돌체를 파괴한다 (내일 구현) 
	}

}
