// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "SimpleShooter.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	mShootingRPM = 600.f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	mTimeBetweenShots = 60 / mShootingRPM;
	mTimeOfLastShot = 0;
}

void AGun::BeginShoot()
{
	float delay = mTimeOfLastShot + mTimeBetweenShots - GetWorld()->TimeSeconds;
	delay = FMath::Clamp(delay, 0.f, mTimeBetweenShots);
	GetWorldTimerManager().SetTimer(mTimerHandle_ShootingTimer, this, &AGun::PullTrigger, mTimeBetweenShots, true, delay);
}

void AGun::EndShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(mTimerHandle_ShootingTimer);
}

void AGun::PullTrigger()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		Server_PullTrigger();
	}

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	AController* OwnerController = GetOwnerContoller();
	if (OwnerController == nullptr) return;
	FHitResult Hit;
	FVector ShotDirection;

	if (GunTrace(OUT Hit, OUT ShotDirection))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation()); 
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location, ShotDirection.Rotation());

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			float DamageToApply = mDamage;

			EPhysicalSurface SurfaceTypeForHit = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			switch (SurfaceTypeForHit)
			{
			case SURFACE_TYPE_FLESH_DEFAULT:
				break;
			case SURFACE_TYPE_FLESH_VULNERABLE:
				DamageToApply *= mHeadshotDamageMultiplier;
				break;
			default:
				break;
			}

			FPointDamageEvent DamageEvent(DamageToApply, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(DamageToApply, DamageEvent, OwnerController, this);
		}

		mTimeOfLastShot = GetWorld()->TimeSeconds;
	}
}

void AGun::Server_PullTrigger_Implementation()
{
	PullTrigger();
}

bool AGun::Server_PullTrigger_Validate()
{
	return true;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerContoller();
	if (OwnerController == nullptr) return false;

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);
	ShotDirection = -ViewPointRotation.Vector();

	FVector EndPoint = ViewPointLocation + ViewPointRotation.Vector() * mMaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	Params.bReturnPhysicalMaterial = true;

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, ViewPointLocation, EndPoint, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerContoller() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}
