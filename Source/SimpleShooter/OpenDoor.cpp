// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/TriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector DoorLocation = GetOwner()->GetActorLocation();
	if (IsOverLapping())
	{
		DoorLocation = FMath::Lerp(DoorLocation, mOpenLocation, mSpeed * DeltaTime);
		if (!mIsOpening)
		{
			UGameplayStatics::PlaySoundAtLocation(this, mOpenDoorSound, GetOwner()->GetActorLocation());
		}
		mIsOpening = true;
	}
	else 
	{
		DoorLocation = FMath::Lerp(DoorLocation, mInitialLocation, mSpeed * DeltaTime);
		if (mIsOpening)
		{
			UGameplayStatics::PlaySoundAtLocation(this, mOpenDoorSound, GetOwner()->GetActorLocation());
		}
		mIsOpening = false;
	}
	GetOwner()->SetActorLocation(DoorLocation);
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindPressurePlate();
	mInitialLocation = GetOwner()->GetActorLocation();
	mOpenLocation = mInitialLocation + (GetOwner()->GetActorForwardVector() * mDistanceToOpen);
}

bool UOpenDoor::IsOverLapping()
{
	if (mPressurePlate)
	{
		TArray<AActor*> OverlappingActors;
		mPressurePlate->GetOverlappingActors(OUT OverlappingActors);
		for (AActor* Actor : OverlappingActors)
		{
			AShooterCharacter* Character = Cast<AShooterCharacter>(Actor);
			if (Character != nullptr)
			{
				return true;
			}
		}
	}

	return false;
}

void UOpenDoor::FindPressurePlate()
{
	if (!mPressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Component %s has no assigned PressurePlate!"), *GetFName().ToString());
	}
}

