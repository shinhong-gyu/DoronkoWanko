// Fll out your copyright notice in the Description page of Project Settings.


#include "HG_MissonStamp.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"

// Sets default values
AHG_MissonStamp::AHG_MissonStamp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 데칼 컴포넌트 생성
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	// 루트로 설정
	SetRootComponent(Decal);

	// 데칼 머터리얼을 랜덤으로 설정하기 위한 난수 생성
	int RandNum = FMath::RandRange(1, 2);

	// 난수에 따른 데칼 머터리얼 경로명
	FString MaterialPath = FString::Printf(TEXT("/Game/HongGyu/M_MissonStamp%d.M_MissonStamp%d"), RandNum, RandNum);

	// 데칼 머터리얼 애셋 로드
	static ConstructorHelpers::FObjectFinder<UMaterial> FindMaterial(*MaterialPath);
	if (FindMaterial.Succeeded())
	{
		// 찾은 머터리얼을 데칼 머터리얼로 설정
		DecalMaterial = FindMaterial.Object;
	}

	// 데칼 컴포넌트의 머터리얼 설정
	Decal->SetMaterial(0, DecalMaterial);

	// 크기 설정
	Decal->DecalSize = FVector(32.0f, 32.0f, 32.0f);

	// zOrder 1로 설정
	Decal->SetSortOrder(1);
}

// Called when the game starts or when spawned
void AHG_MissonStamp::BeginPlay()
{
	Super::BeginPlay();

	// 시작했을 땐 보이지 않도록 설정
	Decal->SetVisibility(false);
}
