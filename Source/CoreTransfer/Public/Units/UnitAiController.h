// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Containers/Queue.h"
#include "UnitAiController.generated.h"

/**
 * 
 */
UCLASS()
class CORETRANSFER_API AUnitAiController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
protected:
	virtual void BeginPlay() override;

private:
	// Temp for testing
	UPROPERTY(EditAnywhere)
	TArray<FVector> WaypointsArray;
	
	TQueue<FVector> Waypoints;

private:
	void MoveToNextWaypoint();
};
