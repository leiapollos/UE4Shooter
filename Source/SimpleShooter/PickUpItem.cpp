// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	mSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spherfe Mesh"));
	mSphereMesh->SetGenerateOverlapEvents(false);
	mSphereMesh->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = mSphereMesh;

	mSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	mSphereCollision->SetupAttachment(mSphereMesh);
	mSphereCollision->InitSphereRadius(160);
	mSphereCollision->SetGenerateOverlapEvents(true);
	mSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnBeginOverlap);
	
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();

}

void APickUpItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, mPickUpEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, mPickUpSound, GetActorLocation());
	Destroy();
}