// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Shoot();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float mRotationRateX = 10.f;
	UPROPERTY(EditAnywhere)
	float mRotationRateY = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float mMaxHealth = 100.f; 
	UPROPERTY(EditDefaultsOnly)
	float mHealth;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* mGun;

	bool mIsShooting = false;
	UPROPERTY(EditAnywhere)
	float mShootingCooldown = 0.15f;
	float mShootingTimer;

	void StartShooting();
	void EndShooting();

	void MoveForward(float AxisValue);
	void MoveSideways(float AxisValue);
	void LookUpJoystick(float AxisValue);
	void LookSidewaysJoystick(float AxisValue);
};
