// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Gun.h"
#include "HealthComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mHealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	mHealthComp->OnHealthChanged.AddDynamic(this, &AShooterCharacter::OnHealthChanged);

	if (GetLocalRole() == ROLE_Authority)
	{
		mGun = GetWorld()->SpawnActor<AGun>(GunClass);
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		mGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		mGun->SetOwner(this);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterCharacter::IsDead() const
{
	if (mHealthComp)
	{
		return mHealthComp->IsDead();
	}
	return false;
}

float AShooterCharacter::GetHealthPercent() const
{
	if (mHealthComp)
	{
		return mHealthComp->GetHealthPercent();
	}
	return 0.f;
}

void AShooterCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0)
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		UE_LOG(LogTemp, Warning, TEXT("Dided"));
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
	if (mGun)
	{
		mGun->PullTrigger();
	}
}

void AShooterCharacter::StartShooting()
{
	if (mGun)
	{
		mGun->BeginShoot();
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("No Gun On Client"));
	}
}

void AShooterCharacter::EndShooting()
{
	if (mGun) 
	{
		mGun->EndShoot();
	}
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

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AShooterCharacter, mGun);
}