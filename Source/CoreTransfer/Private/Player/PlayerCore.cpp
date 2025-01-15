// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCore.h"

#include "Player/PlayerBase.h"

// Sets default values
APlayerCore::APlayerCore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// TODO: The orb will float back to the player base
void APlayerCore::ReturnToBase()
{
	if (HomeBase.IsValid())
	{
		HomeBase->AddCore(this);
	}
}

