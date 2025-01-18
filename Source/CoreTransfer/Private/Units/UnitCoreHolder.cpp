// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCoreHolder.h"

#include "Player/PlayerCore.h"

// Sets default values for this component's properties
UUnitCoreHolder::UUnitCoreHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UUnitCoreHolder::BeginDestroy()
{
	ReturnCore();
	
	Super::BeginDestroy();
}

bool UUnitCoreHolder::SetCore(APlayerCore* NewCore)
{
	if (NewCore == nullptr)
	{
		return false;
	}
	
	if (IsHoldingCore())
	{
		return false;
	}

	Core = NewCore;
	Core->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	return true;
}

void UUnitCoreHolder::ReturnCore()
{
	if (Core != nullptr)
	{
		Core->ReturnToBase();
		Core = nullptr;
	}
}