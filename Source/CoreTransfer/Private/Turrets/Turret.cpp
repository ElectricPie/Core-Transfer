// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets/Turret.h"

#include "Components/SphereComponent.h"
#include "Units/BaseUnit.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(Root);

	RangeVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Range Volume"));
	RangeVolume->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(Root);

	bReplicates = true;
}

void ATurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Get new target if needed
	if (CurrentTarget.IsValid() == false && UnitsInRange.IsEmpty() == false)
	{
		if (UnitsInRange[0].IsValid() == false)
		{
			UnitsInRange.Pop();
		}
		else
		{
			CurrentTarget = UnitsInRange[0];
		}
	}
	
	if (CurrentTarget.IsValid())
	{
	    FVector DirectionToTarget = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		DirectionToTarget.Z = 0.f;
		SetActorRotation(DirectionToTarget.Rotation());
	}
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	RangeVolume->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnRangedEntered);
	RangeVolume->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnRangedLeft);
}


void ATurret::OnRangedEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Handle teams if needed
	if (ABaseUnit* Unit = Cast<ABaseUnit>(OtherActor))
	{
		UnitsInRange.Add(Unit);
		if (Unit->OnUnitDeath.IsAlreadyBound(this, &ATurret::OnUnitDeath) == false)
		{
			Unit->OnUnitDeath.AddDynamic(this, &ATurret::OnUnitDeath);
		}

		if (!CurrentTarget.IsValid())
		{
			CurrentTarget = Unit;
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::Fire, FireRate, true);
		}
	}
}

void ATurret::OnRangedLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ABaseUnit* Unit = Cast<ABaseUnit>(OtherActor))
	{
		UnitsInRange.Remove(Unit);
		Unit->OnUnitDeath.RemoveDynamic(this, &ATurret::OnUnitDeath);
		if (Unit == CurrentTarget && UnitsInRange.IsEmpty() == false)
		{
			CurrentTarget = GetNextValidUnit();
		}
		else if (UnitsInRange.IsEmpty())
		{
			CurrentTarget.Reset();
			GetWorldTimerManager().ClearTimer(FireTimerHandle);
		}
	}
}

void ATurret::Fire()
{
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ATurret::Fire: ProjectileClass is nullptr on %s"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor(ProjectileClass, &ProjectileSpawnPoint->GetComponentTransform(), SpawnParams);
}

void ATurret::OnUnitDeath(ABaseUnit* DeadUnit)
{
	UnitsInRange.Remove(DeadUnit);
	DeadUnit->OnUnitDeath.RemoveDynamic(this, &ATurret::OnUnitDeath);

	if (CurrentTarget == DeadUnit)
	{
		if (UnitsInRange.IsEmpty() == false)
		{
			CurrentTarget = GetNextValidUnit();
		}
		else
		{
			CurrentTarget.Reset();
			GetWorldTimerManager().ClearTimer(FireTimerHandle);
		}
	}
}

ABaseUnit* ATurret::GetNextValidUnit()
{
	TArray<TWeakObjectPtr<ABaseUnit>> UnitsInRangeTemp = UnitsInRange;

	// Find the first valid unit
	for (int i = 0; i < UnitsInRangeTemp.Num(); ++i)
	{
		if (UnitsInRangeTemp[i].IsValid())
		{
			return UnitsInRangeTemp[i].Get();
		}

		// Remove invalid units from original array
		UnitsInRange.Pop();
	}

	return nullptr;
}

