// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UnitCoreHolder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORETRANSFER_API UUnitCoreHolder : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitCoreHolder();

	bool IsHoldingCore() const { return Core.IsValid(); }
	/**
	 * Set the core that this holder is holding
	 * @param NewCore The core to hold
	 * @return true if the core was successfully set, false if the holder is already holding a core or the core is
	 * nullptr
	 */
	UFUNCTION(BlueprintCallable)
	bool SetCore(AActor* NewCore);
	
private:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> Core;
};
