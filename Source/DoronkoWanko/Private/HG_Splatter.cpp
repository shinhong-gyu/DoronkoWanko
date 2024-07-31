// Fill out your copyright notice in the Description page of Project Settings.


#include "HG_Splatter.h"
#include "GW_Player.h"
#include "HG_DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	// ม฿ทย
	Velocity += FVector(0, 0, -980.0f) * DeltaTime;
	UpdataRotation();
}

void AHG_Splatter::SpawnDecalAtLocation(const FVector& Location, const FRotator& Rotation)
{
	if (DecalClass != nullptr) {
		AHG_DecalActor* Decal = GetWorld()->SpawnActor<AHG_DecalActor>(DecalClass, Location, Rotation);
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
		this->Destroy();
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRoation = OtherActor->GetActorRotation();
		this->SpawnDecalAtLocation(SpawnLocation, SpawnRoation);
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