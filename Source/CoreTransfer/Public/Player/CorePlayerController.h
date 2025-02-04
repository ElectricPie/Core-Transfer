// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

USTRUCT()
struct FPreviewBuilding
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* PreviewActor;
	UPROPERTY()
	TSubclassOf<AActor> BuildingClass;
};

/**
 * 
 */
UCLASS()
class CORETRANSFER_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
	void StartBuildingPreview(TSubclassOf<AActor> BuildingClass);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MappingContext;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> SelectAction;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> CancelAction;

	FPreviewBuilding* PreviewBuilding = nullptr;

private:
	bool IsBuildingPreviewActive() const { return nullptr != PreviewBuilding; };
	
	void Move(const FInputActionValue& InputValue);
	void CancelBuildingPreview();
	void ConfirmBuildingPlacement();
};
