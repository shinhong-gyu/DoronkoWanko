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

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->Activate();
	ProjectileMovementComponent->Velocity = FVector(0,0,-1)*500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
}

// Called when the game starts or when spawned
void AHG_Splatter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHG_Splatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHG_Splatter::SpawnDecalAtLocation(const FVector& Location, const FRotator& Rotation)
{
	if (DecalClass != nullptr) {
		AHG_DecalActor* Decal = GetWorld()->SpawnActor<AHG_DecalActor>(DecalClass, Location, Rotation);
		if (nullptr != Decal) {
			UDecalComponent* DecalComp = Decal->GetDecal();
			if (nullptr != DecalComp) {
				DecalComp->DecalSize = FVector(32.0f,32.0f,32.0f);
				DecalComp->SetWorldLocation(Location);
				DecalComp->SetWorldRotation(Rotation);
			}
		}
	}
}

void AHG_Splatter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor != nullptr && OtherActor != this) {
		this->Destroy();
		FVector HitLocation = GetActorLocation();
		FRotator HitRoation = GetActorRotation();
		this->SpawnDecalAtLocation(HitLocation, HitRoation);
	}
}

