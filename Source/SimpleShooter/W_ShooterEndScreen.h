// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ShooterEndScreen.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UW_ShooterEndScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	int mRestartDelay = 3;
};
