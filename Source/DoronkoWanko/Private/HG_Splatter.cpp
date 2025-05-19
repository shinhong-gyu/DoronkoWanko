// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Splatter.h"
#include "GW_Player.h"
#include "HG_DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DoronkoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "HG_MissonStamp.h"
#include "GameFramework/Actor.h"
#include "HJ_RoboticVacuum.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DecalInfoStruct.h"
#include "HG_EnterInstruction.h"
#include "Engine/StaticMesh.h"

// Sets default values
AHG_Splatter::AHG_Splatter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 설정 및 설정
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));

	// 충돌체를 루트 컴포넌트로 설정
	SetRootComponent(SphereComp);

	// 물방울 매시 생성 및 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	// 물방울 매시를 루트 컴포넌트에 부착
	MeshComp->SetupAttachment(RootComponent);

	// Collision 설정
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionProfileName(TEXT("Splatter"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	// 매시 크기를 랜덤으로 설정
	MeshComp->SetRelativeScale3D(FVector(FMath::FRandRange(0.6f, 0.9f), FMath::FRandRange(1.0f, 1.1f), FMath::FRandRange(0.3f, 0.4f)));

	// 초기 변위를 ZeroVector 로 설정
	Velocity = FVector::ZeroVector;

	// 데칼이 물감에 영향을 주지 않도록 설정
	MeshComp->SetReceivesDecals(false);

	// 매시 에셋 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> SplatterMesh(TEXT("/Script/Engine.StaticMesh'/Game/HongGyu/Splatoon/Water_Drop.Water_Drop'"));
	if (SplatterMesh.Succeeded()) {
		MeshComp->SetStaticMesh(SplatterMesh.Object);
	}

	// 머터리얼 애셋 로드
	ConstructorHelpers::FObjectFinder<UMaterial> SplatterMaterial(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Paint_Origin.M_Paint_Origin'"));
	if (SplatterMesh.Succeeded()) {
		MeshComp->SetMaterial(0, SplatterMaterial.Object);
	}

	// 생성할 물감(데칼)의 모양을 랜덤으로 설정
	int32 RandValue = FMath::RandRange(1, 5);
	FString MaterialPath = FString::Printf(TEXT("/Game/HongGyu/Splatoon/M_Paint%d.M_Paint%d"), RandValue, RandValue);

	// 데칼 모양 에셋 로드
	ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterial(*MaterialPath);
	if (DecalMaterial.Succeeded()) {
		SelectedDecalMaterial = DecalMaterial.Object;
	}

	// 수명을 3초로 설정 (맵을 뚫고 떨어지는 버그 발생 시 알아서 삭제되도록 함.)
	this->SetLifeSpan(3.0f);


}

// Called when the game starts or when spawned
void AHG_Splatter::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHG_Splatter::OnMyBeginOverlap);

}

// Called every frame
void AHG_Splatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// p = p0 * vt
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);

	// 위치 업데이트
	SetActorLocation(NewLocation);

	// 중력 적용
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;

	// 회전 업데이트
	UpdataRotation();

	// 매시의 머터리얼 가져오기
	UMaterialInterface* MaterialInterface = MeshComp->GetMaterial(0);

	// 만약 MaterialInterface이 nullptr이 아니라면
	if (MaterialInterface)
	{
		// 다이나믹 머터리얼을 생성하기 위해 머터리얼 인터페이스를 형변환
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);

		// 만약 다이나믹 머터리얼이 nullptr이라면
		if (!DynamicMaterial)
		{
			// 새롭게 다이나믹 머터리얼을 생성
			DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		}

		// 만약 다이나믹 머터리얼이 제대로 생성되었다면
		if (DynamicMaterial)
		{
			// 다이나믹 머터리얼에 색을 설정하고
			DynamicMaterial->SetVectorParameterValue("Color", MyColor);

			// 다이나믹 머터리얼을 물방울 매시의 머터리얼로 설정.
			MeshComp->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AHG_Splatter::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌한 액터가 로봇 청소기인 경우 아무것도 하지 않음
	if (OtherActor->IsA<ARoboticVacuum>())
	{
		return;
	}

	// 데칼 정보
	FDecalInfo SpawnDeaclInfo = FDecalInfo();

	// 물방울이 충돌을 일으켰을 때 충돌 근처에 있는 데칼의 정보를 저장하는 변수
	FDecalInfo* HittedDecalInfo;

	// 임의의 범위를 생성하기 위한 난수
	float RandNum = FMath::FRandRange(100.0f, 150.0f);

	// 점수를 조작하기 위해 GameMode를 가져와 저장.
	TObjectPtr<ADoronkoGameMode> GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	// 1~9점 사이의 랜덤 정수 생성 (점수)
	int32 RandScore = FMath::RandRange(1, 9);

	// 점수 업데이트
	GM->AddScore(RandScore);

	// UI 업데이트
	GM->UpdataScoreBoard();

	// 물방울이 충돌했을 때 물방울의 머리 방향에 있는 물체에 데칼을 생성하기 위해 LineTrace를 사용
	// LineTrace 파라미터
	FVector Start = GetActorLocation();
	FVector End = Start + Velocity.GetSafeNormal() * 10000;
	FHitResult HitInfo;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams QParams;
	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	Params.AddIgnoredActor(this);

	// LineTrace
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitInfo, Start, End, QParams, Params);
	if (bHit)
	{
		// 물방울 충돌 위치 내에 데칼이 있는지 확인
		HittedDecalInfo = IsDecalInRange(HitInfo.ImpactPoint, RandNum, RandNum);

		// 만약 충돌한 물방울이 청소기에 의해 스폰된 것이 아니면서 충돌한 위치에 데칼이 존재하면서 해당 데칼의 색이 물방울의 색과 같다면
		if (bSpawnedByRV == false && HittedDecalInfo != nullptr && MyColor == HittedDecalInfo->Color)
		{
			// 기존 데칼의 크기를 키운다.
			HittedDecalInfo->DecalComp->SetRelativeScale3D(FVector(1.0f, 1.3f, 1.3f));
		}
		// 아니라면
		else
		{
			// 데칼을 새로 생성
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedDecalMaterial, FVector(-5.0f, RandNum, RandNum), OtherComp, NAME_None, HitInfo.ImpactPoint, HitInfo.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition);

			// 만약 데칼이 제대로 생성되었다면
			if (Decal)
			{
				// 데칼 머터리얼을 다이나믹 머터리얼로 생성
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Decal->GetDecalMaterial(), this);

				// 다이나믹 머터리얼이 제대로 생성되었다면
				if (DynamicMaterial)
				{
					// 다이나믹 머터리얼에 색을 설정하고
					DynamicMaterial->SetVectorParameterValue("Color", MyColor);

					// 데칼의 머터리얼을 다이나믹 머터리얼로 설정하고
					Decal->SetDecalMaterial(DynamicMaterial);

					// 데칼의 순서를 정하고
					Decal->SetSortOrder(SortOrder);

					// 다음 데칼의 순서를 업데이트
					SortOrder++;
				}
			}

			// 데칼 정보를 저장
			SpawnDeaclInfo.DecalComp = Decal;
			SpawnDeaclInfo.Color = MyColor;
			SpawnDeaclInfo.Pos = HitInfo.ImpactPoint;

			// GameMode가 관리하는 Decal 배열에 추가
			GM->SpawnedDecalArr.Add(SpawnDeaclInfo);
		}
	}

	// 액터 삭제
	this->Destroy();

	// 물방울이 충돌한 곳의 범위 안에 있는 스탬프 찾기
	TArray<AHG_MissonStamp*> StampArray = IsStampInRange(HitInfo.ImpactPoint, RandNum, RandNum);

	// 만약 찾은 스탬프의 개수가 0 이상이라면
	if (StampArray.Num() > 0)
	{
		// StampArray를 순회하면서
		for (auto Stamp : StampArray)
		{
			// Stamp의 Decal이 보이지 않는 상태라면
			if (Stamp->Decal->GetVisibleFlag() == false)
			{
				// Stamp의 Decal을 보이도록하고
				Stamp->Decal->SetVisibility(true);

				// 찾은 스탬프 개수를 증가시킨다.
				GM->StampCount++;

				// 플레이어를 가져와서
				auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

				// 플레이어의 미니맵 UI에 스탬프 정보를 업데이트 한다.
				if (Player->MinimapUI)
				{
					Player->MinimapUI->MiniMapUpdate(Stamp->StampID);
				}
			}
		}
	}
}


FVector AHG_Splatter::ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal)
{
	// 정사영 계산, Vector의 수직 성분 제거
	return Vector - FVector::DotProduct(Vector, PlaneNormal) * PlaneNormal;
}

void AHG_Splatter::Initalize(FVector initVeloccity)
{
	// 속도 초기화
	Velocity = initVeloccity;
}

void AHG_Splatter::UpdataRotation()
{
	// 속도가 0에 가까우면 회전하지 않음
	if (!Velocity.IsNearlyZero()) {
		// 속도가 0에 가깝지 않다면

		// 속도를 회전값으로 바꾸고
		FRotator NewRotation = Velocity.Rotation();

		// 액터를 회전시킴
		SetActorRotation(NewRotation);
	}
}

TArray<AHG_MissonStamp*> AHG_Splatter::IsStampInRange(FVector Pos, float Param1, float Param2)
{
	// 찾은 스탬프들을 저장하기 위한 배열
	TArray<AActor*> StampArray;

	// 반환할 스탬프 배열 
	TArray<AHG_MissonStamp*> Result;

	// 레벨에 있는 모든 스탬프를 가져옴
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StampClass, StampArray);

	UE_LOG(LogTemp, Error, TEXT("%d"), StampArray.Num());

	// 찾은 스탬프들을 순회하면서
	for (auto Stamp : StampArray)
	{
		// 거리 계산
		FVector Dist = Pos - (Stamp->GetActorLocation());

		// 만약 스탬프의 위치와 물방울의 위치 사이의 거리가 Param1, Param2보다 작다면
		if ((Param1 / 2) * (Param1 / 2) + (Param2 / 2) * (Param2 / 2) > Dist.Size() * Dist.Size())
		{
			UE_LOG(LogTemp, Error, TEXT("%s"), *Stamp->GetName());
			// 스탬프를 Result에 추가
			Result.Emplace(Cast<AHG_MissonStamp>(Stamp));
		}
	}

	// 반환
	return Result;
}

void AHG_Splatter::SetMyColor(FLinearColor Value)
{
	// 색 설정
	MyColor = Value;
}

FDecalInfo* AHG_Splatter::IsDecalInRange(FVector Pos, float Param1, float Param2)
{
	// 거리 벡터
	FVector Dist;

	// 가장 가까운 데칼을 찾기 위한 변수
	FVector Min = FVector(1000.0f, 1000.0f, 1000.0f);

	// 반환할 데칼 인덱스
	int32 ReturnIndex = -1;

	// GameMode 가져오기
	ADoronkoGameMode* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	// GameMode가 존재한다면
	if (GM)
	{
		// GameMode의 SpawnedDecalArr를 순회하면서
		for (int i = 0; i < GM->SpawnedDecalArr.Num(); i++)
		{
			// Pos에서 데칼으로의 벡터 계산
			Dist = Pos - GM->SpawnedDecalArr[i].DecalComp->GetComponentLocation();
			// 만약 계산한 Dist를 반지름으로 하는 원이 Param1, Param2을 두 반지름으로 하는 타원 안에 있다면
			if ((Param1 / 2) * (Param1 / 2) + (Param2 / 2) * (Param2 / 2) > Dist.Size() * Dist.Size())
			{
				// 만약 현재 저장되어있는 최소 거리보다 현재 계산한 거리가 더 작다면
				if (Min.Size() >= Dist.Size())
				{
					// 최소 거리 값을 갱신하고
					Min = Dist;

					// 최소 거리에 있는 데칼의 인덱스로 반환 인덱스를 업데이트
					ReturnIndex = i;
				}
			}
		}

		// 만약 찾은 데칼의 인덱스가 0보다 크고 SpawnedDecalArr의 개수보다 작다면
		if (ReturnIndex > 0 && ReturnIndex < GM->SpawnedDecalArr.Num())
		{
			// 찾은 데칼의 정보를 반환
			return &(GM->SpawnedDecalArr[ReturnIndex]);
		}
	}
	// GM을 가져오는데 실패하거나 찾은 데칼이 없다면
	return nullptr;
}

