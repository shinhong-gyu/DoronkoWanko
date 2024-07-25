// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Train.h"
#include "Components/BoxComponent.h"
#include "HJ_TrainWheel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_Train::AHJ_Train()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(40.0f, 60.0f, 40.0f));
	// 메쉬 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

	// 충돌체 처리 
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHJ_Train::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AHJ_Train::OnOverlapEnd);

	// 열차 회전 변수 
	Radius = 500.0f;
	AngularSpeed = 2.0f;
	CurrentAngle = 0.0f;

	// 회전 기본값 지정 
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

	/*if (IsInRange)
	{*/
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::E))
		{
			EquipSocket();
			/*PressE = PressE + 1;*/
		}


	// 기차 회전 로직 -> Socket 구현되면 조건부 재생 
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

void AHJ_Train::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 바퀴와 충돌 인식 
	auto* Wheel = Cast<AHJ_TrainWheel>(OtherActor);

	if (Wheel)
	{
		IsInRange = true;
		UE_LOG(LogTemp, Warning, TEXT("WheelHit"));
	}
}

void AHJ_Train::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 바퀴와 충돌 종료 인식 
	auto* Wheel = Cast<AHJ_TrainWheel>(OtherActor);

	if (Wheel)
	{
		IsInRange = false;
		UE_LOG(LogTemp, Warning, TEXT("WheelOut"));
	}
}

void AHJ_Train::EquipSocket()
{
	// 기차 바퀴를 소켓에 붙인다 
	// 다만 현재 바퀴도& 기차도 모두 Static Mesh 상태로 열차에 붙질 않는다 
	FName WheelSocket(TEXT("WheelSocket"));

	if (CurrentWheel == nullptr)
	{
		CurrentWheel = GetWorld()->SpawnActor<AHJ_TrainWheel>(WheelFactory, GetActorLocation(), FRotator::ZeroRotator);

		if (CurrentWheel)
		{ 
			CurrentWheel->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WheelSocket);
		}	
	}
	
	// 대안1. Scene 컴포넌트 만들어서 해당 위치에 바퀴가 생성되게 한다. 
	// 대안2. 바퀴를 hidden 설정 -> 인터렉션 할 때 마다 show // 이때 열차의 Rotation 정상으로 돌아오게 
}



