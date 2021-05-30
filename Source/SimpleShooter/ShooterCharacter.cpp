// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	mGun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	mGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	mGun->SetOwner(this);
	
	mHealth = mMaxHealth;

	mShootingTimer = 0.f;
}
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mShootingTimer -= DeltaTime;
	if (mIsShooting && mShootingTimer <= 0)
	{
		Shoot();
		mShootingTimer = mShootingCooldown;
	}

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookSideways"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpJoystick"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookSidewaysJoystick"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::StartShooting);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AShooterCharacter::EndShooting);
}

void AShooterCharacter::Shoot()
{
	mGun->PullTrigger();
}

void AShooterCharacter::StartShooting()
{
	mIsShooting = true;
}

void AShooterCharacter::EndShooting()
{
	mIsShooting = false;
	mShootingTimer = 0.f;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveSideways(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpJoystick(float AxisValue)
{
	AddControllerPitchInput(AxisValue * mRotationRateY * GetWorld()->DeltaTimeSeconds);
}

void AShooterCharacter::LookSidewaysJoystick(float AxisValue)
{
	AddControllerYawInput(AxisValue * mRotationRateX * GetWorld()->DeltaTimeSeconds);
}

float AShooterCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamageApplied = mHealth;
	mHealth = FMath::Clamp(mHealth - DamageToApply, 0.f, mMaxHealth);
	ActualDamageApplied -= mHealth;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f : %f"), mHealth, ActualDamageApplied);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
	}

	return ActualDamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	return mHealth <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return (mHealth / mMaxHealth);
}
