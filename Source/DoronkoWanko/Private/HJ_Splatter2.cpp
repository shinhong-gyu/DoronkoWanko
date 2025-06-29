// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Splatter2.h"
#include "GW_Player.h"
#include "HG_DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DoronkoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "HG_MissonStamp.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AHJ_Splatter2::AHJ_Splatter2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionProfileName(TEXT("Splatter2"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	Velocity = FVector::ZeroVector;
	MeshComp->SetReceivesDecals(false);

	int32 RandValue = FMath::RandRange(1, 5);
	FString MaterialPath = FString::Printf(TEXT("/Game/HongGyu/Splatoon/M_Paint%d.M_Paint%d"), RandValue, RandValue);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/HongGyu/Splatoon/Water_Drop.Water_Drop'"));
	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial11(TEXT("/Script/Engine.Material'/Game/Material/BaseMaterials/M_Paint_Origin.M_Paint_Origin'"));
	if (TempMaterial11.Succeeded()) {
		MeshComp->SetMaterial(0, TempMaterial11.Object);
	}

	int32 RandPaint = FMath::RandRange(1, 5);
	FString MatPath = FString::Printf(TEXT("/Game/HongGyu/Splatoon/M_Paint%d.M_Paint%d"), RandPaint, RandPaint);

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(*MatPath);
	if (TempMaterial.Succeeded()) {
		SelectedDecalMaterial= TempMaterial.Object;
	}

	this->SetLifeSpan(3.0f);
}

// Called when the game starts or when spawned
void AHJ_Splatter2::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_Splatter2::OnMyBeginOverlap);
}

// Called every frame
void AHJ_Splatter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UMaterialInterface* MaterialInterface = MeshComp->GetMaterial(0);
	if (MaterialInterface) {
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MaterialInterface);
		if (!DynamicMaterial) 
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
		}
		if (DynamicMaterial) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Splatter2 My Color : %s"), *(MyColor.ToString()));
			DynamicMaterial->SetVectorParameterValue("Color", MyColor);
			MeshComp->SetMaterial(0, DynamicMaterial);
		}
	}
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(NewLocation);

	// ม฿ทย
	UE_LOG(LogTemp, Warning, TEXT("Splatter2 : %s"), *MyColor.ToString());
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;
	UpdataRotation();
}

void AHJ_Splatter2::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRoation;

	float RandNum = FMath::FRandRange(100.0f, 150.0f);
	auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());

	int32 RanInt = FMath::RandRange(1, 9);
	GM->SetScore(RanInt);
	GM->UpdataScoreBoard();
	FVector end = SpawnLocation + Velocity.GetSafeNormal() * 10000;

	FHitResult HitInfo;
	FCollisionQueryParams params;
	FCollisionObjectQueryParams QParams;
	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitInfo, SpawnLocation, end, QParams, params);
	if (bHit)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedDecalMaterial, FVector(-5.0f, RandNum, RandNum), OtherComp, NAME_None, HitInfo.ImpactPoint, HitInfo.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition);
		if (Decal) 
		{
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Decal->GetDecalMaterial(), this);

			if (DynamicMaterial)
			{
				DynamicMaterial->SetVectorParameterValue("Color", MyColor);
				Decal->SetDecalMaterial(DynamicMaterial);
				Decal->SetSortOrder(SortOrder);
				SortOrder++;
			}
		}
	}

	TArray<TObjectPtr<AHG_MissonStamp>> StampArray = IsStampInRange(HitInfo.ImpactPoint, RandNum, RandNum);
	if (StampArray.Num() != 0) 
	{
		for (auto Stamp : StampArray)
		{
			if (Stamp->Decal->GetVisibleFlag() == false) 
			{
				Stamp->Decal->SetVisibility(true);
				GM->StampCount++;
				auto* Player = Cast<AGW_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				if (Player) 
				{
					if (Player->MinimapUI) 
					{
						Player->MinimapUI->MiniMapUpdate(Stamp->StampID);
						UGameplayStatics::PlaySound2D(GetWorld(), StampSFX);
					}
				}
			}
		}
	}
	this->Destroy();
}

FVector AHJ_Splatter2::ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal)
{
	return Vector - FVector::DotProduct(Vector, PlaneNormal) * PlaneNormal;
}

void AHJ_Splatter2::SetMyColor(FLinearColor Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Splatter2 : SetMyColor"), *Value.ToString());
	MyColor = Value;
}

void AHJ_Splatter2::Initalize(FVector initVeloccity)
{
	Velocity = initVeloccity;
}

void AHJ_Splatter2::UpdataRotation()
{
	if (!Velocity.IsNearlyZero())
	{
		FRotator NewRotation = Velocity.Rotation();
		SetActorRotation(NewRotation);
	}
}

TArray<TObjectPtr<AHG_MissonStamp>> AHJ_Splatter2::IsStampInRange(FVector Pos, float Param1, float Param2)
{
	TArray<AActor*> StampArray;

	TArray< TObjectPtr<AHG_MissonStamp>> Result;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StampClass, StampArray);
	for (auto a : StampArray) 
	{
		FVector Dist = Pos - (a->GetActorLocation());

		if (Param1* Param1 > Dist.Size() * Dist.Size()) 
		{
			Result.Add(Cast<AHG_MissonStamp>(a));
		}
	}
	return Result;
}

