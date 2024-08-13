// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Train.h"
#include "Components/BoxComponent.h"
#include "HJ_TrainWheel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "GW_Player.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHJ_Train::AHJ_Train()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 잉크 스폰 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(BoxComp);

	// 열차 바퀴 메쉬 생성
	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel1->SetupAttachment(MeshComp);
	Wheel2->SetupAttachment(MeshComp);

	// 바퀴 안보이게 설정 
	Wheel1->SetVisibility(false);
	Wheel2->SetVisibility(false);

	// 충돌체 처리
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 열차 회전 변수 
	Radius = 850.0f;
	AngularSpeed = 0.8f;
	CurrentAngle = 0.0f;

	// 회전 기본값 지정 
	RotationSpeed = 46.0f;
	CurrentRotationAngel = 0.0f;
	// 위젯 문구 생성 
	InteractionText = FText::FromString(TEXT("Interaction"));
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스트 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// 기차 바퀴 캐스트
	CurrentWheel = Cast<AHJ_TrainWheel>(UGameplayStatics::GetActorOfClass(GetWorld(), WheelFactory));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AHJ_Train::OnOverlapBegin);
}

// Called every frame
void AHJ_Train::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WheelCheck == 1)
	{
		Wheel1->SetVisibility(true);
	}

	if (WheelCheck == 2)
	{
		Wheel2->SetVisibility(true);
	}

	if (WheelCheck > 2)
	{
		SoundCheck++;
		CurrentAngle += AngularSpeed * DeltaTime;
		float X = Radius * FMath::Cos(CurrentAngle);
		float Y = Radius * FMath::Sin(CurrentAngle);
		float Z = GetActorLocation().Z;

		SetActorLocation(FVector(X - 1650.f, Y - 4300.f, Z)); // 위치 정해지면 FVector(X,Y,Z) 더해주기 

		CurrentRotationAngel += RotationSpeed * DeltaTime;
		if (CurrentRotationAngel > 360.0f)
		{
			CurrentRotationAngel -= 360.0f;
		}
		FRotator TrainRotation = FRotator(0.0f, CurrentRotationAngel, 0.0f);
		BoxComp->SetRelativeRotation(TrainRotation);

		CurrTime += DeltaTime;

		if (CurrTime > MakeTime)
		{
			// 물감 스폰하기 (정수리에서 Ink 가 Spawn 되도록)

			FTransform T = InkArrow->GetComponentTransform();
			auto* Ink = GetWorld()->SpawnActor<AHG_Splatter>(InkFactory, T);

			if (nullptr != Ink) {
				Ink->MeshComp->SetVisibility(false);
				Ink->Initalize(FVector(-100, 0, 0));
			}
			CurrTime = 0;
		}
	}

	if (SoundCheck > 0 && SoundCheck < 2)
	{
		playSFX();
	}
}

void AHJ_Train::playSFX()
{
	UGameplayStatics::PlaySound2D(GetWorld(), TrainSFX);
}

void AHJ_Train::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA<AGW_Player>())
	{
		auto* Player = Cast<AGW_Player>(OtherActor);
		if (Player) {
			if (Player->AttachedStaticObject && Player->AttachedStaticObject->IsA<AHJ_TrainWheel>()) {
				Player->AttachedStaticObject->Destroy();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("CheckWhy"));
	}
	 //바퀴가 함께 충돌할 때만 상호작용 가능 
	/*UE_LOG(LogTemp, Warning, TEXT("CheckWhy"));*/

// 	if (OtherActor->IsA<AHJ_TrainWheel>())
// 	{
// 		AbleInteract = true;
// 		// (방법 수정 필요) 부딪히고 E키 누를 때만 파괴 가능하게 
// 		OtherActor->Destroy();
// 	}
}

void AHJ_Train::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("12 %s"),*OtherActor->GetClass()->GetName());
	//if (OtherActor->IsA<AHJ_TrainWheel>())
	//{
	//	AbleInteract = true;
	//	// (방법 수정 필요) 부딪히고 E키 누를 때만 파괴 가능하게 
	//	OtherActor->Destroy();
	//}
}


void AHJ_Train::InteractionWith()
{
	//if (AbleInteract)
	//{
		WheelCheck += 1;
		// 열차 바퀴를 파괴하는 함수 필요 
		/*CurrentWheel->Destroy();*/
		AbleInteract = false;
	/*}*/
}

void AHJ_Train::ItemDrop()
{
	bTurnOn = false;
}



