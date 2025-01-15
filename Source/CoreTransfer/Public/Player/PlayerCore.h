// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCore.generated.h"

class APlayerBase;

UCLASS()
class CORETRANSFER_API APlayerCore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCore();

	void ReturnToBase();

public:
	// The player base this core will return to if it is not collected
	TWeakObjectPtr<APlayerBase> HomeBase;
};
