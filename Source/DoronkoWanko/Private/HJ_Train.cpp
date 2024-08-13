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

	// ��ũ ���� 
	InkArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InkArrow"));
	InkArrow->SetupAttachment(BoxComp);

	// ���� ���� �޽� ����
	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	Wheel1->SetupAttachment(MeshComp);
	Wheel2->SetupAttachment(MeshComp);

	// ���� �Ⱥ��̰� ���� 
	Wheel1->SetVisibility(false);
	Wheel2->SetVisibility(false);

	// �浹ü ó��
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionProfileName(TEXT("MapObject"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ȸ�� ���� 
	Radius = 850.0f;
	AngularSpeed = 0.8f;
	CurrentAngle = 0.0f;

	// ȸ�� �⺻�� ���� 
	RotationSpeed = 46.0f;
	CurrentRotationAngel = 0.0f;
	// ���� ���� ���� 
	InteractionText = FText::FromString(TEXT("Interaction"));
}

// Called when the game starts or when spawned
void AHJ_Train::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ��Ʈ 
	GW_Player = Cast<AGW_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// ���� ���� ĳ��Ʈ
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

		SetActorLocation(FVector(X - 1650.f, Y - 4300.f, Z)); // ��ġ �������� FVector(X,Y,Z) �����ֱ� 

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
			// ���� �����ϱ� (���������� Ink �� Spawn �ǵ���)

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
	 //������ �Բ� �浹�� ���� ��ȣ�ۿ� ���� 
	/*UE_LOG(LogTemp, Warning, TEXT("CheckWhy"));*/

// 	if (OtherActor->IsA<AHJ_TrainWheel>())
// 	{
// 		AbleInteract = true;
// 		// (��� ���� �ʿ�) �ε����� EŰ ���� ���� �ı� �����ϰ� 
// 		OtherActor->Destroy();
// 	}
}

void AHJ_Train::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("12 %s"),*OtherActor->GetClass()->GetName());
	//if (OtherActor->IsA<AHJ_TrainWheel>())
	//{
	//	AbleInteract = true;
	//	// (��� ���� �ʿ�) �ε����� EŰ ���� ���� �ı� �����ϰ� 
	//	OtherActor->Destroy();
	//}
}


void AHJ_Train::InteractionWith()
{
	//if (AbleInteract)
	//{
		WheelCheck += 1;
		// ���� ������ �ı��ϴ� �Լ� �ʿ� 
		/*CurrentWheel->Destroy();*/
		AbleInteract = false;
	/*}*/
}

void AHJ_Train::ItemDrop()
{
	bTurnOn = false;
}



