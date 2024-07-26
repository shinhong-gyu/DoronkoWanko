// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Train.h"
#include "Components/BoxComponent.h"
#include "HJ_TrainWheel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "GW_Player.h"

// Sets default values
AHJ_Train::AHJ_Train()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� 
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(40.0f, 60.0f, 40.0f));
	// �޽� ����
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

	// ���� �޽� ���� 
	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel3"));
	Wheel4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel4"));

	Wheel1->SetupAttachment(BoxComp);
	Wheel2->SetupAttachment(BoxComp);
	Wheel3->SetupAttachment(BoxComp);
	Wheel4->SetupAttachment(BoxComp);

	// ���� �޽� ó���� �Ⱥ��̰� ���� 
	Wheel2->SetVisibility(false);
	Wheel3->SetVisibility(false);
	Wheel4->SetVisibility(false);

	// �浹ü ó�� 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_Train::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_Train::OnOverlapEnd);

	// ���� ȸ�� ���� 
	Radius = 500.0f;
	AngularSpeed = 2.0f;
	CurrentAngle = 0.0f;

	// ȸ�� �⺻�� ���� 
	RotationSpeed = 115.0f;
	CurrentRotationAngel = 0.0f;
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHJ_Train::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInRange)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->WasInputKeyJustPressed(EKeys::E))
		{
			PressE = PressE + 1;
		}
	}

	// ���� ȸ�� ���� -> Socket �����Ǹ� ���Ǻ� ��� 
	if (PressE == 1)
	{
		Wheel2->SetVisibility(true);
	}
	else if (PressE == 2)
	{
		Wheel3->SetVisibility(true);
	}
	else if (PressE >= 3)
	{
		Wheel4->SetVisibility(true);

		CurrentAngle += AngularSpeed * DeltaTime;
		float X = Radius * FMath::Cos(CurrentAngle);
		float Y = Radius * FMath::Sin(CurrentAngle);
		float Z = GetActorLocation().Z;

		SetActorLocation(FVector(X, Y, Z));

		CurrentRotationAngel += RotationSpeed * DeltaTime;
		if (CurrentRotationAngel > 360.0f)
		{
			CurrentRotationAngel -= 360.0f;
		}
		FRotator TrainRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
		BoxComp->SetRelativeRotation(TrainRotation);
	}
	else 
	{ }
}

void AHJ_Train::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �÷��̾�� �浹 �ν� 
	auto* Player = Cast<AGW_Player>(OtherActor);

	if (Player)
	{
		IsInRange = true;
		UE_LOG(LogTemp, Warning, TEXT("WheelHit"));
	}
}

void AHJ_Train::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �÷��̾�� �浹 ���� �ν� 
	auto* Player = Cast<AHJ_TrainWheel>(OtherActor);

	if (Player)
	{
		IsInRange = false;
		UE_LOG(LogTemp, Warning, TEXT("WheelOut"));
	}
}



