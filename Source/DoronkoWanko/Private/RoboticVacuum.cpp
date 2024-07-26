// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ARoboticVacuum::ARoboticVacuum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� 
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoxComp"));
	SetRootComponent(SphereComp);

	// �޽� ���� 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// ȸ�� �⺻�� ���� 
	RotationSpeed = 20.0f;
	CurrentRotationAngle = 0.0f;

}

// Called when the game starts or when spawned
void ARoboticVacuum::BeginPlay()
{
	Super::BeginPlay();
	CheckAngle = GetActorRotation();
	
}

// Called every frame
void ARoboticVacuum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*CheckAngle = CurrentRotationAngle - ActorAngle;*/
	// �������� - ���밪���� ����ؾ��� 
	
	// �߰��Ǵ� ȸ������ 45��, 90��, 135�� 3���� �� ���������� ������ 
	if (CurrentRotationAngle <= 45.0f) // R���� ���������� ���� �� 
	{
		/*CurrentRotationAngle = RobotRotation.Yaw; */// ������ �� ��ġ���� ȸ���ؾ� �� 
		// ���� ȸ�� ���� - ����� ȸ�� ���� -> ���ο� ������ ��� // �ش� ������ ���� 45���� ���� �� ���� 
		CurrentRotationAngle += RotationSpeed * DeltaTime;
	}
	else
	{
		Speed = 300;
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
		RotationSpeed = 0;
		
	}
	FRotator VacuumRotation = CheckAngle + FRotator(0.0f, CurrentRotationAngle, 0.0f);
	SphereComp->SetRelativeRotation(VacuumRotation);

}

void ARoboticVacuum::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor)
	{
		CurrentRotationAngle = 0;
		Speed = 0;
		CheckAngle = GetActorRotation();
		SetActorLocation(GetActorLocation() + GetActorForwardVector()*(-10.0f));
		UE_LOG(LogTemp, Warning, TEXT("VacuumOUT"));
	}
}



//void ARoboticVacuum::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor)
//	{
//	CurrentRotationAngel = 0;
//	Speed = 0;
//	UE_LOG(LogTemp, Warning, TEXT("VacuumOUT"));
//	}
//}

