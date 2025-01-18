// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/BaseUnit.h"

#include "Units/UnitCoreHolder.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

float ABaseUnit::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		// Return any cores to base
		TArray<UUnitCoreHolder*> CoreHolders;
		GetComponents<UUnitCoreHolder>(CoreHolders);
		for (UUnitCoreHolder* CoreHolder : CoreHolders)
		{
			CoreHolder->ReturnCore();
		}
		
		OnUnitDeath.Broadcast(this);
		Destroy();
	}
	
	return DamageAmount;
}