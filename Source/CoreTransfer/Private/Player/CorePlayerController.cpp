// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CorePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/PlayerPawn.h"

void ACorePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Building Preview
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
			FVector SnappedLocation = FVector(
				FMath::Floor(HitResult.Location.X / GridSpacing) * GridSpacing,
				FMath::Floor(HitResult.Location.Y / GridSpacing) * GridSpacing,
				HitResult.Location.Z
				);
			PreviewBuilding->PreviewActor->SetActorLocation(SnappedLocation);
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
		NewPreviewBuilding->SetReplicates(false);
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

	ServerPlaceBuilding(PreviewBuilding->BuildingClass, BuildingSpawnLocation);

	PreviewBuilding = nullptr;
}

void ACorePlayerController::ServerPlaceBuilding_Implementation(TSubclassOf<AActor> BuildingClass,
                                                               const FVector& Location)
{
	if (!HasAuthority())
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BuildingClass, Location, FRotator::ZeroRotator, SpawnParameters);
}

bool ACorePlayerController::ServerPlaceBuilding_Validate(TSubclassOf<AActor> BuildingClass, const FVector& Location)
{
	return true;
}
