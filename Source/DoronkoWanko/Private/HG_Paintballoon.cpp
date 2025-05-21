// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Paintballoon.h"
#include "HG_Splatter.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GW_Player.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AHG_Paintballoon::AHG_Paintballoon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sphere 메시 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	// 만약 로드에 성공했다면
	if (TempMesh.Succeeded()) 
	{
		// 메시 컴포넌트로 로드한 오브젝트를 설정
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	// PaintBall 머터리얼 로드
	static ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(TEXT("/Game/HongGyu/Splatoon/M_PaintBall.M_PaintBall"));

	// 만약 로드에 성공했다면
	if (TempMaterial.Succeeded()) 
	{
		// 메시 컴포넌트의 첫 번째 머터리얼로 로드한 오브젝트를 설정
		MeshComp->SetMaterial(0, TempMaterial.Object);
	}

	// 랜덤한 색깔 설정
	MyColor = FLinearColor::MakeRandomColor();

	// 충돌체의 콜리젼 설정을 MapObject로 설정
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));

	// 메시는 충돌을 일으키지 않도록 설정
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AHG_Paintballoon::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면 레벨에 나와있는 모든 페인트 풍선의 색을 랜덤한 색으로 설정

	// 메시의 머터리얼의 포인터를 가져온다.
	UMaterialInterface* MaterialInterface = MeshComp->GetMaterial(0);

	// 성공적으로 머터리얼을 불러왔다면
	if (MaterialInterface)
	{
		// 불러온 머터리얼을 다이나믹 머터리얼로 캐스트해주고
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);

		// 만약 캐스트에 실패했다면
		if (DynamicMaterial == nullptr)
		{
			// 다이나믹 머터리얼로 만들어준다.
			DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		}

		// 다이나믹 머터리얼로 만드는데 성공했다면
		if (DynamicMaterial)
		{
			// 다이나믹 머터리얼의 Color 파라미터를 랜덤으로 만든 색깔로 설정
			DynamicMaterial->SetVectorParameterValue("Color", MyColor);

			// 메시의 0번 머터리얼을 다이나믹 머터리얼로 설정
			MeshComp->SetMaterial(0, DynamicMaterial);
		}
	}
}

// Called every frame
void AHG_Paintballoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHG_Paintballoon::InteractionWith()
{
	// 물방울의 초기속도
	FVector InitialVelocity;

	// 물방울 스폰 트랜스폼
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 만약 위젯이 생성된 상태라면
	if (Widget != nullptr)
	{
		// 위젯을 화면에서 지운다.
		Widget->RemoveFromParent();
	}
	
	// 페인트 볼을 레벨에서 지운다
	Destroy();

	// 5개의 물방울 생성
	for (int i = 0; i < 5; i++)
	{
		// 랜덤 속도 생성
		InitialVelocity = FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), FMath::RandRange(300, 600));

		// 물방울 액터(AHG_Splatter) 스폰
		AHG_Splatter* Splatter = GetWorld()->SpawnActor<AHG_Splatter>(SplatterFactory, SpawnLocation, SpawnRotation);

		// 만약 Splatter 가 정상적으로 스폰되었다면
		if (Splatter)
		{
			// 속도를 초기화 하고
			Splatter->Initalize(InitialVelocity);

			// 페인트 공의 색을 물방울 색으로 설정
			Splatter->SetMyColor(MyColor);
		}

		// 플레이어가 바라보고 있는 액터를 비워준다
		if (AGW_Player* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			Player->LookAtActor = nullptr;
		}
	}
}


