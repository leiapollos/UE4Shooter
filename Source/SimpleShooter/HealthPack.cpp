// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"

// Sets default values
AHealthPack::AHealthPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPack::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetFName().ToString());
		FPointDamageEvent DamageEvent(-mHealthToRestore, SweepResult, -SweepResult.ImpactNormal, nullptr);
		OtherActor->TakeDamage(-mHealthToRestore, DamageEvent, nullptr, nullptr);
	}

	//Super destroys the object so it should be called at the end
	Super::OnBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

