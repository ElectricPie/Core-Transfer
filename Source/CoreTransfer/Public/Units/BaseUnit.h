// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDeathSignature, ABaseUnit*, DeadUnit);

UCLASS()
class CORETRANSFER_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUnitDeathSignature OnUnitDeath;
	
public:
	// Sets default values for this character's properties
	ABaseUnit();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere)
	float Health = 100.f;
	
};
