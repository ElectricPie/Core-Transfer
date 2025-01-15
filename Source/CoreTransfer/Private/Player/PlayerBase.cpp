// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBase.h"

#include "Components/BoxComponent.h"
#include "Units/BaseUnit.h"
#include "Units/UnitCoreHolder.h"

// Sets default values
APlayerBase::APlayerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	CoreHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Core Holder"));
	CoreHolder->SetupAttachment(MeshComponent);

	CoreCollectionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Core Collection Volume"));
	CoreCollectionVolume->SetupAttachment(RootComponent);
}

void APlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator NewRotation = CoreHolder->GetComponentRotation();
	NewRotation.Yaw += CoreRotationSpeed * DeltaSeconds * 360; // 360 degrees per rotation
	CoreHolder->SetWorldRotation(NewRotation);
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	CoreCollectionVolume->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnCollectionVolumeBeginOverlap);

	if (CoreClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCore: CoreClass is not set on %s"), *GetName());
		return;
	}

	for (uint32 i = 0; i < InitialCoreCount; ++i)
	{
		if (AActor* NewCore = GetWorld()->SpawnActor<AActor>(CoreClass))
		{
			// Calculate the position of the orbs in a circle around the core holder
			float Angle = 360 / InitialCoreCount * i;
			Angle = 2 * PI * (FMath::Clamp<float>(Angle, 0, 360) / 360);
			FVector Pos = FVector(
				OrbDistanceFromCentre * FMath::Cos(Angle),
				OrbDistanceFromCentre * FMath::Sin(Angle),
				0);

			NewCore->SetActorLocation(CoreHolder->GetComponentLocation() + Pos);
			NewCore->AttachToComponent(CoreHolder, FAttachmentTransformRules::KeepWorldTransform);
			Cores.Add(NewCore);
		}
	}
}

void APlayerBase::OnCollectionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cores.Num() == 0)
	{
		return;
	} 
	
	TArray<UUnitCoreHolder*> OtherCoreHolders;
	OtherActor->GetComponents<UUnitCoreHolder>(OtherCoreHolders);

	for (const auto& OtherCoreHolder : OtherCoreHolders)
	{
		if (!OtherCoreHolder->IsHoldingCore())
		{
			if (OtherCoreHolder->SetCore(Cores.Last().Get()))
			{
				Cores.RemoveAt(Cores.Num() - 1);
			}
		}
	}
}
