// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawner.generated.h"

UCLASS()
class CORETRANSFER_API AUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawner();
	
	UFUNCTION(BlueprintCallable)
	void AuthSpawnUnit(const TSubclassOf<ACharacter>& UnitToSpawn);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> UnitSpawnLocation;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0f, UIMin=0.0f))
	float SpawnInterval = 1.0f;
	
	TQueue<TSubclassOf<ACharacter>> SpawnQueue;
	FTimerHandle SpawnTimerHandle;

private:
	void AuthSpawnNextUnit();
};
