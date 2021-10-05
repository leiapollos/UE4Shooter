// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	
	void BeginShoot();
	void EndShoot();

	UFUNCTION()
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	float mMaxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	float mDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	int mHeadshotDamageMultiplier = 3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	float mTimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	float mShootingRPM;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	float mTimeOfLastShot;

	FTimerHandle mTimerHandle_ShootingTimer;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerContoller() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PullTrigger();
};
