// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboticVacuum.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ARoboticVacuum::ARoboticVacuum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoxComp"));
	SetRootComponent(SphereComp);

	// 메쉬 생성 
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// 회전 기본값 지정 
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
	// 각도차이 - 절대값으로 계산해야함 
	
	// 추가되는 회전값은 45도, 90도, 135도 3가지 중 랜덤값으로 나간다 
	if (CurrentRotationAngle <= 45.0f) // R값에 랜덤값으로 들어가야 함 
	{
		/*CurrentRotationAngle = RobotRotation.Yaw; */// 엑터의 현 위치에서 회전해야 함 
		// 이전 회전 각도 - 변경된 회전 각도 -> 새로운 변수에 담기 // 해당 변수의 값이 45보다 작을 때 동작 
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

