// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickUpItem.h"
#include "HealthPack.generated.h"

UCLASS()
class SIMPLESHOOTER_API AHealthPack : public APickUpItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();

	UPROPERTY(EditAnywhere)
	float mHealthToRestore = 10.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:	
};
