// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBase.generated.h"

class APlayerCore;
class UBoxComponent;

UCLASS()
class CORETRANSFER_API APlayerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBase();

	virtual void Tick(float DeltaSeconds) override;
	
	void AddCore(APlayerCore* NewCore);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> CoreHolder;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CoreCollectionVolume;

	UPROPERTY(EditAnywhere, meta=(ClampMin=0, UIMin=0))
	uint32 InitialCoreCount = 10;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0.f, UIMin=0.f))
	float OrbDistanceFromCentre = 100.f;
	UPROPERTY(EditAnywhere)
	float CoreRotationSpeed = 0.2f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerCore> CoreClass;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<APlayerCore>> Cores;

private:
	/**
	 * Calculates the position of a core so that it is positioned evenly around the CoreHolder relative to the total
	 * number of cores
	 * @param TotalCores The total number of cores
	 * @param CoreIndex The relative index of the core
	 * @return The position of the core
	 */
	FVector CalculateCorePosition(uint32 TotalCores, uint32 CoreIndex) const;

	/**
	 * Moves all cores to be in their correct position relative to the number of cores
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RepositionAllCores();
	
	UFUNCTION()
	void OnCollectionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										const FHitResult& SweepResult);
};
