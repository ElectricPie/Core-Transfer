// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CorePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerPawn.h"

void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(MappingContext, 0);
	}
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACorePlayerController::Move);
	}
}

void ACorePlayerController::Move(const FInputActionValue& InputValue)
{
	const FVector2D MoveDirection = InputValue.Get<FVector2D>();
	if (APlayerPawn* PlayerPawn = Cast<APlayerPawn>(GetPawn()))
	{
		PlayerPawn->Move(MoveDirection);
	}
}
