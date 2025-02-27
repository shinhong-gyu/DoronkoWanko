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

	ConstructorHelpers::FObjectFinder<UMaterial> TempMaterial(*MaterialPath);
	if (TempMaterial.Succeeded()) {
		SelectedMaterial = TempMaterial.Object;
	}

	this->SetLifeSpan(3.0f);
}

// Called when the game starts or when spawned
void AHJ_Splatter2::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_Splatter2::OnMyBeginOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &AHJ_Splatter2::OnMyHit);
}

// Called every frame
void AHJ_Splatter2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(NewLocation);

	// �߷�
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;
	UpdataRotation();
}

void AHJ_Splatter2::SpawnDecalAtLocation(const FVector& Location, const FRotator& Rotation)
{
	if (DecalClass != nullptr) {
		AHG_DecalActor* Decal = GetWorld()->SpawnActor<AHG_DecalActor>(DecalClass, Location, Rotation);
		auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
		int32 RandNum = FMath::RandRange(1, 9);
		GM->SetScore(RandNum);
		GM->UpdataScoreBoard();
		if (nullptr != Decal) {
			UDecalComponent* DecalComp = Decal->GetDecal();
			if (nullptr != DecalComp) {
				DecalComp->SetWorldLocation(Location);
				DecalComp->SetWorldRotation(Rotation);
			}
		}
	}
}

void AHJ_Splatter2::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRoation;
	this->Destroy();
	float RandNum = FMath::FRandRange(100.0f, 150.0f);
	auto* GM = Cast<ADoronkoGameMode>(GetWorld()->GetAuthGameMode());
	int32 RanInt = FMath::RandRange(1, 9);
	GM->SetScore(RanInt);
	GM->UpdataScoreBoard();
	FVector end = SpawnLocation + Velocity.GetSafeNormal() * 10000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	FCollisionObjectQueryParams QParams;
	QParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByObjectType(hitInfo, SpawnLocation, end, QParams, params);
	if (bHit)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedMaterial, FVector(-5.0f, RandNum, RandNum), OtherComp, NAME_None, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition);
		//GetWorld()->SpawnActor<AActor>(NormalArrow, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator());
		//DrawDebugLine(GetWorld(), hitInfo.ImpactPoint, (hitInfo.ImpactNormal * 10000.0f), FColor::Blue, false, 10000.0f);
		//GetWorld()->SpawnActor<AActor>(NormalArrow, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator());

		float RandRotZ = FMath::FRandRange(0.0f, 360.0f);

		//Decal->AddRelativeRotation(FRotator(0,0,RandRotZ));
	}
	TArray<AHG_MissonStamp*> StampArray = IsStampInRange(hitInfo.ImpactPoint, RandNum, RandNum);
	if (StampArray.Num() != 0) {
		for (auto s : StampArray) {
			s->Decal->SetVisibility(true);
		}
	}

}
void AHJ_Splatter2::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	this->Destroy();
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRoation = UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal);
	float RandNum = FMath::FRandRange(100.0f, 150.0f);
	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedMaterial, FVector(-15.0f, RandNum, RandNum), OtherComp, NAME_None, SpawnLocation, SpawnRoation, EAttachLocation::KeepWorldPosition);
	if (Decal) {
		Decal->AddRelativeRotation(FRotator(FMath::FRandRange(0.0f, 360.0f), 0, 0));
	}
}


FVector AHJ_Splatter2::ProjectVectorOntoPlane(const FVector& Vector, const FVector& PlaneNormal)
{
	return Vector - FVector::DotProduct(Vector, PlaneNormal) * PlaneNormal;
}

void AHJ_Splatter2::Initalize(FVector initVeloccity)
{
	Velocity = initVeloccity;
}

void AHJ_Splatter2::UpdataRotation()
{
	if (!Velocity.IsNearlyZero()) {
		FRotator NewRotation = Velocity.Rotation();
		SetActorRotation(NewRotation);
	}
}

TArray<AHG_MissonStamp*> AHJ_Splatter2::IsStampInRange(FVector Pos, float Param1, float Param2)
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

