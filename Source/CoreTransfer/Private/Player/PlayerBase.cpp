// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBase.h"

#include "Components/BoxComponent.h"
#include "Player/PlayerCore.h"
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

void APlayerBase::AddCore(APlayerCore* NewCore)
{
	if (NewCore == nullptr)
		return;

	NewCore->AttachToComponent(CoreHolder, FAttachmentTransformRules::KeepWorldTransform);
	Cores.Add(NewCore);
	RepositionAllCores();
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

	// Spawn Initial orbs
	for (uint32 i = 0; i < InitialCoreCount; ++i)
	{
		if (APlayerCore* NewCore = GetWorld()->SpawnActor<APlayerCore>(CoreClass))
		{
			FVector Pos = CalculateCorePosition(InitialCoreCount, i);

			NewCore->SetActorLocation(CoreHolder->GetComponentLocation() + Pos);
			NewCore->AttachToComponent(CoreHolder, FAttachmentTransformRules::KeepWorldTransform);
			NewCore->HomeBase = this;
			Cores.Add(NewCore);
		}
	}
}

FVector APlayerBase::CalculateCorePosition(const uint32 TotalCores, const uint32 CoreIndex) const
{
	float Angle = 360 / TotalCores * CoreIndex;
	Angle = 2 * PI * (FMath::Clamp<float>(Angle, 0, 360) / 360);

	return FVector(
		OrbDistanceFromCentre * FMath::Cos(Angle),
		OrbDistanceFromCentre * FMath::Sin(Angle),
		0);
}

void APlayerBase::RepositionAllCores()
{
	for (int32 i = 0; i < Cores.Num(); ++i)
	{
		FVector Pos = CalculateCorePosition(Cores.Num(), i);
		Cores[i]->SetActorLocation(CoreHolder->GetComponentLocation() + Pos);
	}
}

void APlayerBase::OnCollectionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	if (Cores.Num() == 0)
	{
		return;
	}

	// Assign cores to passing holders
	TArray<UUnitCoreHolder*> OtherCoreHolders;
	OtherActor->GetComponents<UUnitCoreHolder>(OtherCoreHolders);
	for (const auto& OtherCoreHolder : OtherCoreHolders)
	{
		if (!OtherCoreHolder->IsHoldingCore())
		{
			if (OtherCoreHolder->SetCore(Cores.Last().Get()))
			{
				Cores.RemoveAt(Cores.Num() - 1);

				// Reposition the remaining orbs
				RepositionAllCores();
			}
		}
	}
}
