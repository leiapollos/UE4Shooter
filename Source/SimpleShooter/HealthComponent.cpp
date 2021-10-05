// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	mDefaultHealth = 100.f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	mHealth = mDefaultHealth;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);
	}
}

void UHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	float DamageToApply = Damage;
	float ActualDamageApplied = mHealth;
	mHealth = FMath::Clamp(mHealth - DamageToApply, 0.f, mDefaultHealth);
	ActualDamageApplied -= mHealth;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f : %f"), mHealth, ActualDamageApplied);

	OnHealthChanged.Broadcast(this, mHealth, ActualDamageApplied, DamageType, InstigatedBy, DamageCauser);
}

bool UHealthComponent::IsDead() const
{
	return mHealth <= 0;
}

float UHealthComponent::GetHealthPercent() const
{
	return (mHealth / mDefaultHealth);
}