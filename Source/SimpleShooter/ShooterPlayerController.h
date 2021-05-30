// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus  = nullptr , bool bIsWinner  = false ) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UW_ShooterEndScreen> mLostScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UW_ShooterEndScreen> mWinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> mHUDClass;

	UPROPERTY(EditAnywhere)
	float mRestartDelay = 5.f;

	FTimerHandle mRestartTimer;

	UUserWidget* mHUDScreen = nullptr;

	bool mIsRestarting = false;
};
