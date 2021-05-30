// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
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
			if (!Hit.BoneName.Compare("head"))
			{
				UE_LOG(LogTemp, Warning, TEXT("bone: %s"), *Hit.BoneName.ToString());
				DamageToApply *= mHeadshotDamageMultiplier;
			}
			FPointDamageEvent DamageEvent(DamageToApply, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(DamageToApply, DamageEvent, OwnerController, this);
		}
	}
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

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, ViewPointLocation, EndPoint, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerContoller() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}
