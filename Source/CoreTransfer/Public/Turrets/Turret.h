// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class ABaseUnit;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVerticalDirectionChanged, float, NewDirection);

UCLASS()
class CORETRANSFER_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnVerticalDirectionChanged OnVerticalDirectionChangedEvent;
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<  FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> TurretMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> RangeVolume;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

	UPROPERTY(VisibleInstanceOnly)
	TArray<TWeakObjectPtr<ABaseUnit>> UnitsInRange;
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<ABaseUnit> CurrentTarget;
	
	FTimerHandle FireTimerHandle;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.1f, UIMin=0.1f))
	float FireRate = 1.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(Replicated)
	FVector DirectionToTarget;

private:
	UFUNCTION()
	void OnRangedEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRangedLeft(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire();
	
	UFUNCTION()
	void OnUnitDeath(ABaseUnit* DeadUnit);

	ABaseUnit* GetNextValidUnit();
};
