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

	Velocity = FVector::ZeroVector;
	MeshComp->SetReceivesDecals(false);
	
	int32 RandValue = FMath::RandRange(1,5);
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
	SphereComp->OnComponentHit.AddDynamic(this, &AHG_Splatter::OnMyHit);
}

// Called every frame
void AHG_Splatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(NewLocation);

	// ม฿ทย
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;
	UpdataRotation();
}

void AHG_Splatter::SpawnDecalAtLocation(const FVector& Location, const FRotator& Rotation)
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

void AHG_Splatter::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRoation;
	this->Destroy();
	float RandNum = FMath::FRandRange(100.0f, 150.0f);
;	if (NormalArrow)
	{
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
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(SelectedMaterial, FVector(-15.0f, RandNum, RandNum), OtherComp, NAME_None, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator(), EAttachLocation::KeepWorldPosition);
			//GetWorld()->SpawnActor<AActor>(NormalArrow, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator());
			//DrawDebugLine(GetWorld(), hitInfo.ImpactPoint, (hitInfo.ImpactNormal * 10000.0f), FColor::Blue, false, 10000.0f);
			//GetWorld()->SpawnActor<AActor>(NormalArrow, hitInfo.ImpactPoint, hitInfo.ImpactNormal.ToOrientationRotator());

			float RandRotZ = FMath::FRandRange(0.0f, 360.0f);

			//Decal->AddRelativeRotation(FRotator(0,0,RandRotZ));
		}
	}

}
void AHG_Splatter::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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