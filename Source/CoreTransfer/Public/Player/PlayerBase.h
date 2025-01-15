// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBase.generated.h"

class UBoxComponent;

UCLASS()
class CORETRANSFER_API APlayerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBase();

	virtual void Tick(float DeltaSeconds) override;

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
	TSubclassOf<AActor> CoreClass;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>> Cores;

private:
	UFUNCTION()
	void OnCollectionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										const FHitResult& SweepResult);
};
