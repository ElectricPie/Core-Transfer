// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/UnitCoreHolder.h"

// Sets default values for this component's properties
UUnitCoreHolder::UUnitCoreHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UUnitCoreHolder::SetCore(AActor* NewCore)
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

