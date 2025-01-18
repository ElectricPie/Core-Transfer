// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitAiController.h"

#include "Navigation/PathFollowingComponent.h"

void AUnitAiController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.Code == EPathFollowingResult::Success)
	{
		MoveToNextWaypoint();
	}
}

void AUnitAiController::BeginPlay()
{
	for (FVector Waypoint : WaypointsArray)
	{
		Waypoints.Enqueue(Waypoint);
	}

	Super::BeginPlay();

	MoveToNextWaypoint();
}

void AUnitAiController::MoveToNextWaypoint()
{
	FVector NextWaypoint;
	if (Waypoints.Dequeue(NextWaypoint))
	{
		MoveToLocation(NextWaypoint);
	}
}
