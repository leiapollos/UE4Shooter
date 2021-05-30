// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

class ATriggerVolume;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void FindPressurePlate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual bool IsOverLapping();

 	UPROPERTY(EditAnywhere, Category = "Effects")
 	USoundBase* mOpenDoorSound;

	UPROPERTY(EditAnywhere, Category = "Door")
	ATriggerVolume* mPressurePlate = nullptr;

	UPROPERTY(EditAnywhere, Category = "Door", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float mSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float mDistanceToOpen = 2.0f;

	bool mIsOpening = false;

	FVector mInitialLocation;
	FVector mOpenLocation;
		
};
