// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 1500.f;
	CameraArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
}

void APlayerPawn::Move(FVector2D MoveDirection)
{
	
	const FVector DeltaLocation = FVector(MoveDirection.X, MoveDirection.Y, 0.f) * MoveSpeedModifier * UGameplayStatics::GetWorldDeltaSeconds(this);

	const FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + GetTransform().TransformVector(DeltaLocation);
	
	SetActorLocation(NewLocation);
}
