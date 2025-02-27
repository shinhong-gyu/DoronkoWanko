// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_FanWing.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "HJ_FanWingSplatter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHJ_FanWing::AHJ_FanWing()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    WingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WingArrow"));
    WingArrow->SetupAttachment(RootComponent);

    RotationSpeed = 200.0f;
    CurrentRotationAngel = 0.0f;

    // 위젯 문구 생성 
    InteractionText = FText::FromString(TEXT("INTERACTION"));
}

// Called when the game starts or when spawned
void AHJ_FanWing::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AHJ_FanWing::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bTurnOn)
    {
        CurrentRotationAngel += RotationSpeed * DeltaTime;
        if (CurrentRotationAngel > 360.0f)
        {
            CurrentRotationAngel -= 360.0f;
        }

        FRotator WingRotation = FRotator(CurrentRotationAngel, 0.0f, 0.0f);
        BoxComp->SetRelativeRotation(WingRotation);
    }


}

void AHJ_FanWing::InteractionWith()
{
    bTurnOn = !bTurnOn;

    if (bTurnOn)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), FanWingSFX);
        SpawnedSplatter = GetWorld()->SpawnActor<AHJ_FanWingSplatter>(WingSplatter, WingArrow->GetComponentTransform());
    }
    else
    {
        if (SpawnedSplatter)
        {
            // 전원을 끄면 바람 충돌체를 제거한다 
            SpawnedSplatter->Destroy();
            SpawnedSplatter = nullptr;
        }
    }

}

