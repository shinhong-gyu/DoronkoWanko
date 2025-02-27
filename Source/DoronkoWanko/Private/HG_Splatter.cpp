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
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionProfileName(TEXT("Splatter"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	//FMath::FRandRange
	MeshComp->SetRelativeScale3D(FVector(FMath::FRandRange(0.6f,0.9f), FMath::FRandRange(1.0f, 1.1f), FMath::FRandRange(0.3f, 0.4f)));

	Velocity = FVector::ZeroVector;
	MeshComp->SetReceivesDecals(false);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/HongGyu/Splatoon/Water_Drop.Water_Drop'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial11(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Paint_Origin.M_Paint_Origin'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetMaterial(0, TempMaterial11.Object);
	}

	int32 RandValue = FMath::RandRange(1, 5);
	FString MaterialPath = FString::Printf(TEXT("/Game/HongGyu/Splatoon/M_Paint%d.M_Paint%d"), RandValue, RandValue);

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(*MaterialPath);
	if (TempMaterial.Succeeded()) {
		SelectedMaterial = TempMaterial.Object;
	}

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
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(NewLocation);

	// �߷�
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;
	UpdataRotation();

	UMaterialInterface* MaterialInterface = MeshComp->GetMaterial(0);
	if (MaterialInterface) {
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
		if (!DynamicMaterial) {
			DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		}
		if (DynamicMaterial) {
			DynamicMaterial->SetVectorParameterValue("Color", MyColor);
			MeshComp->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AHG_Splatter::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ARoboticVacuum>()) {
		return;
	}
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRoation;
	FDecalInfo SpawnDeaclInfo = FDecalInfo();
	FDecalInfo* HittedDecalInfo;
	float RandNum = FMath::FRandRange(100.0f, 150.0f);
	auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
	int32 RanInt = FMath::RandRange(1, 9);
	FVector end = SpawnLocation + Velocity.GetSafeNormal() * 10000;

	FHitResult hitInfo;
	FCollisionQueryParams params;
	FCollisionObjectQueryParams QParams;


	this->Destroy();
	GM->SetScore(RanInt);
	GM->UpdataScoreBoard();

	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(hitInfo, SpawnLocation, end, QParams, params);
	if (bHit)
	{
		HittedDecalInfo = IsDecalInRange(hitInfo.ImpactPoint, RandNum, RandNum);
		if (bSpawnedByRV == false && HittedDecalInfo != nullptr && MyColor == HittedDecalInfo->Color) {
			UE_LOG(LogTemp, Warning, TEXT("SpawnedBy = false"));
			HittedDecalInfo->DecalComp->SetRelativeScale3D(FVector(1.0f, 1.3f, 1.3f));

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("bSpawnedBy = true"));
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedMaterial, FVector(-5.0f, RandNum, RandNum), OtherComp, NAME_None, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition);
			if (Decal) {
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Decal->GetDecalMaterial(), this);
				if (DynamicMaterial) {
					DynamicMaterial->SetVectorParameterValue("Color", MyColor);
					Decal->SetDecalMaterial(DynamicMaterial);
					Decal->SetSortOrder(SO);
					SO++;
				}
			}
			SpawnDeaclInfo.DecalComp = Decal;
			SpawnDeaclInfo.Color = MyColor;
			SpawnDeaclInfo.Pos = hitInfo.ImpactPoint;
			GM->SpawnedDecalArr.Add(SpawnDeaclInfo);
		}
	}
	TArray<AHG_MissonStamp*> StampArray = IsStampInRange(hitInfo.ImpactPoint, RandNum, RandNum);
	if (StampArray.Num() != 0) {
		for (auto s : StampArray) {
			if (s->Decal->GetVisibleFlag() == false) {
				s->Decal->SetVisibility(true);
				GM->StampCount++;
				auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				Player->MinimapUI->MiniMapUpdate(s->StampID);
			}
		}
	}
}


FVector AHG_Splatter::ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal)
{
	return Vector - FVector::DotProduct(Vector, PlaneNormal) * PlaneNormal;
}

void AHG_Splatter::Initalize(FVector initVeloccity)
{
	Velocity = initVeloccity;
}

void AHG_Splatter::UpdataRotation()
{
	if (!Velocity.IsNearlyZero()) {
		FRotator NewRotation = Velocity.Rotation();
		SetActorRotation(NewRotation);
	}
}

TArray<AHG_MissonStamp*> AHG_Splatter::IsStampInRange(FVector Pos, float Param1, float Param2)
{
	TArray<AActor*> StampArray;
	TArray<AHG_MissonStamp*> Result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StampFactory, StampArray);
	for (auto a : StampArray) {
		FVector Dist = Pos - (a->GetActorLocation());
		if ((Param1 / 2) * (Param1 / 2) + (Param2 / 2) * (Param2 / 2) > Dist.Size() * Dist.Size()) {
			Result.Add(Cast<AHG_MissonStamp>(a));
		}
	}
	return Result;
}

void AHG_Splatter::SetMyColor(FLinearColor Value)
{
	MyColor = Value;
}

FDecalInfo* AHG_Splatter::IsDecalInRange(FVector Pos, float Param1, float Param2)
{
	FVector Dist;
	FVector Min = FVector(1000.0f, 1000.0f, 1000.0f);
	int32 RetIdx = -1;
	ADoronkoGameMode* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
	for (int i = 0; i < GM->SpawnedDecalArr.Num(); i++) {
		Dist = Pos - GM->SpawnedDecalArr[i].DecalComp->GetComponentLocation();
		// ���� ���ϴ� �Ÿ� �ȿ� ������
		if ((Param1 / 2) * (Param1 / 2) + (Param2 / 2) * (Param2 / 2) > Dist.Size() * Dist.Size()) {
			// ���� ����� DecalCompã��
			// Min �� ������Ʈ
			if (Min.Size() >= Dist.Size()) {
				Min = Dist;
				RetIdx = i;
			}
		}
	}
	if (RetIdx > 0 && RetIdx < GM->SpawnedDecalArr.Num())
	{
		//GM->SpawnedDecalArr[RetIdx].DecalComp->DecalSize += FVector(0.0f, 1000.0f, 1000.0f);
		return &(GM->SpawnedDecalArr[RetIdx]);
	}
	else
		return nullptr;//FDecalInfo();
}

