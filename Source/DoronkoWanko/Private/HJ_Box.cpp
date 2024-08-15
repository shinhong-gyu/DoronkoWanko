// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Box.h"
#include "Components/BoxComponent.h"
#include "HJ_Cake.h"
#include "GW_Player.h"

// Sets default values
AHJ_Box::AHJ_Box()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// 충돌체 설정
	MeshComp->OnComponentHit.AddDynamic(this, &AHJ_Box::OnHit);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AHJ_Box::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHJ_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Vec = GetActorLocation();
	Rot = GetActorRotation();
}

void AHJ_Box::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA<AGW_Player>())
	{
	// 케이크는 컷씬 찍을 때만 투명하게 (unvisible, 이후 visible) 
		this->Destroy();
		/*GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, Vec-FVector(0,0,780), Rot-FRotator(0,0,0));*/
		/*GetWorld()->SpawnActor<AHJ_ElectricFan>(ElectricFanFactory, Vec - FVector(0, 0, 640), Rot - FRotator(0, -90, 0));*/
		GetWorld()->SpawnActor<AHJ_WhaleHat>(HatFactory, Vec - FVector(0, 0, 780), Rot - FRotator(0, -90, 0));
	}
}

