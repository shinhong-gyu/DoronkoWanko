// Fill out your copyright notice in the Description page of Project Settings.

#include "HJ_TestInk.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AHJTestInk::AHJTestInk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� 
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	// �޽� ���� 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// �浹���� 
	//BoxComp->SetCollisionProfileName(TEXT("BlockAll"));
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �߻�ü ������Ʈ ���� 
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(RootComponent);

	MovementComp->InitialSpeed = 300;
	MovementComp->MaxSpeed = 300;
	MovementComp->bShouldBounce = true;


}

// Called when the game starts or when spawned
void AHJTestInk::BeginPlay()
{
	Super::BeginPlay();
	// 2�� �Ŀ� ��ũ�� �ı��ǰ� �ϰ� �ʹ�(Timer �̿�)
		FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda([&]()
		{ this->Destroy(); }), 2.0f, false);
	
}

// Called every frame
void AHJTestInk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

