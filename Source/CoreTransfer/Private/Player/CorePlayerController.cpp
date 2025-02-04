// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CorePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerPawn.h"

void ACorePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsBuildingPreviewActive())
	{
		if (nullptr == PreviewBuilding->PreviewActor)
		{
			UE_LOG(LogTemp, Error, TEXT("ACorePlayerController::Tick: PreviewBuilding Actor is nullptr"));
			PreviewBuilding = nullptr;
			return;
		}
		
		FVector WorldLocation;
		FVector WorldDirection;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FHitResult HitResult;
		const FVector StartLocation = WorldLocation;
		const FVector EndLocation = WorldLocation + WorldDirection * 10000.f;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation,
		                                         ECollisionChannel::ECC_Visibility, CollisionQueryParams))
		{
			PreviewBuilding->PreviewActor->SetActorLocation(HitResult.Location);
		}
	}
}

void ACorePlayerController::StartBuildingPreview(TSubclassOf<AActor> BuildingClass)
{
	if (IsBuildingPreviewActive())
	{
		return;
	}

	if (nullptr == BuildingClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ACorePlayerController::StartBuildingPreview: BuildingClass is nullptr"));
		return;
	}

	FVector WorldLocation;
	FVector WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FHitResult HitResult;
	FVector StartLocation = WorldLocation;
	FVector EndLocation = WorldLocation + WorldDirection * 10000.f;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility,
	                                         CollisionQueryParams))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewPreviewBuilding = GetWorld()->SpawnActor<AActor>(BuildingClass, HitResult.Location,
		                                                            FRotator::ZeroRotator,
		                                                            SpawnParameters);
		NewPreviewBuilding->SetActorTickEnabled(false);
		PreviewBuilding = new FPreviewBuilding{NewPreviewBuilding, BuildingClass};
	}
}

void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(MappingContext, 0);
	}

	SetShowMouseCursor(true);
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACorePlayerController::Move);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this,
		                                   &ACorePlayerController::CancelBuildingPreview);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this,
		                                   &ACorePlayerController::ConfirmBuildingPlacement);
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

void ACorePlayerController::CancelBuildingPreview()
{
	if (IsBuildingPreviewActive())
	{
		if (PreviewBuilding->PreviewActor)
		{
			PreviewBuilding->PreviewActor->Destroy();
		}
		PreviewBuilding = nullptr;
	}
}

void ACorePlayerController::ConfirmBuildingPlacement()
{
	if (false == IsBuildingPreviewActive())
	{
		return;
	}
	
	if (nullptr == PreviewBuilding->PreviewActor)
	{
		return;
	}

	const FVector BuildingSpawnLocation = PreviewBuilding->PreviewActor->GetActorLocation();
	PreviewBuilding->PreviewActor->Destroy();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(PreviewBuilding->BuildingClass, BuildingSpawnLocation, FRotator::ZeroRotator,
	                               SpawnParameters);

	PreviewBuilding = nullptr;
}
