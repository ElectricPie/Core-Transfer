// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UnitCoreHolder.generated.h"


class APlayerCore;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORETRANSFER_API UUnitCoreHolder : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitCoreHolder();

	virtual void BeginDestroy() override;

	bool IsHoldingCore() const { return Core.IsValid(); }
	/**
	 * Set and move the core to this
	 * @param NewCore The core to hold
	 * @return true if the core was successfully set, false if the holder is already holding a core or the core is
	 * nullptr
	 */
	UFUNCTION(BlueprintCallable)
	bool SetCore(APlayerCore* NewCore);
	UFUNCTION(BlueprintCallable)
	void ReturnCore();
	
private:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<APlayerCore> Core;
};
