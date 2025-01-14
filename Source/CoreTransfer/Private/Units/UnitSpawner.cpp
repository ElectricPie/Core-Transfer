// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitSpawner.h"

#include "GameFramework/Character.h"
#include "Units/UnitAiController.h"

// Sets default values
AUnitSpawner::AUnitSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	UnitSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Unit Spawn Location"));
	UnitSpawnLocation->SetupAttachment(RootComponent);
}

void AUnitSpawner::SpawnUnit(const TSubclassOf<ACharacter>& UnitToSpawn)
{
	if (UnitToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UnitToSpawn is nullptr"));
		return;
	}

	SpawnQueue.Enqueue(UnitToSpawn);

	if (!SpawnTimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AUnitSpawner::SpawnNextUnit, SpawnInterval, true);
	}
}

void AUnitSpawner::SpawnNextUnit()
{
	if (SpawnQueue.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	TSubclassOf<ACharacter> UnitToSpawn;
	SpawnQueue.Dequeue(UnitToSpawn);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ACharacter* NewActor = GetWorld()->SpawnActor<ACharacter>(UnitToSpawn, UnitSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, SpawnParams);
	NewActor->SpawnDefaultController();
	
	if (AUnitAiController* AiController = Cast<AUnitAiController>(NewActor->GetInstigatorController()))
	{
		AiController->MoveToNextWaypoint();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AUnitSpawner: Unit spawned without AI controller"));
		Destroy(NewActor);
	}
}

