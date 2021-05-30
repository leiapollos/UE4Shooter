// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "W_ShooterEndScreen.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	mHUDScreen = CreateWidget(this, mHUDClass);
	if (mHUDScreen != nullptr)
	{
		mHUDScreen->AddToViewport();
	}

	mIsRestarting = false;
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (mIsRestarting) return; //Return right away if we are already restarting


	if (mHUDScreen != nullptr)
	{
		mHUDScreen->RemoveFromViewport();
	}

	if (bIsWinner)
	{
		UW_ShooterEndScreen* WinScreen = Cast<UW_ShooterEndScreen>(CreateWidget(this, mWinScreenClass));
		if (WinScreen != nullptr)
		{
			WinScreen->mRestartDelay = mRestartDelay;
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UW_ShooterEndScreen* LoseScreen = Cast<UW_ShooterEndScreen>(CreateWidget(this, mLostScreenClass));
		if (LoseScreen != nullptr)
		{
			LoseScreen->mRestartDelay = mRestartDelay;
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(mRestartTimer, this, &APlayerController::RestartLevel, mRestartDelay);

	mIsRestarting = true;
}