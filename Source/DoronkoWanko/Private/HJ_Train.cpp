// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Train.h"
#include "Components/BoxComponent.h"
#include "HJ_TrainWheel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "GW_Player.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "HJ_TrainBody.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHJ_Train::AHJ_Train()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 
	//BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	//SetRootComponent(BoxComp);
	//BoxComp->SetBoxExtent(FVector(40.0f, 60.0f, 40.0f));
	// 메쉬 생성
	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);*/

	// 기차 연결고리 
	/*FirstConstraintComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FirstConstraintComp"));
	FirstConstraintComp->SetupAttachment(RootComponent);

	SecondConstraintComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("SecondConstraintComp"));
	SecondConstraintComp->SetupAttachment(RootComponent);*/

	// 열차 회전 변수 
	Radius = 500.0f;
	AngularSpeed = 2.0f;
	CurrentAngle = 0.0f;

	// 회전 기본값 지정 
	RotationSpeed = 115.0f;
	CurrentRotationAngel = 0.0f;
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스트 
	/*GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));*/

	//InteractionText = FText::FromString(TEXT("E) COMEON"));
	
}

// Called every frame
void AHJ_Train::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bTurnOn)
	//{
	//	if (GW_Player)
	//	{
	CurrentAngle += AngularSpeed * DeltaTime;
	float X = Radius * FMath::Cos(CurrentAngle);
	float Y = Radius * FMath::Sin(CurrentAngle);
	float Z = GetActorLocation().Z;

	SetActorLocation(FVector(X, Y, Z)); // 위치 정해지면 FVector(X,Y,Z) 더해주기 

	CurrentRotationAngel += RotationSpeed * DeltaTime;
	if (CurrentRotationAngel > 360.0f)
	{
		CurrentRotationAngel -= 360.0f;
	}
	FRotator TrainRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
	BoxComp->SetRelativeRotation(TrainRotation);
			/*	}
			}*/
	// 	}
}



