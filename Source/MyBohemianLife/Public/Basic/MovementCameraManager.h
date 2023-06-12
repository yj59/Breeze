// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MovementCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class MYBOHEMIANLIFE_API AMovementCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) float CrouchBlendDuration=.2f;
	float CrouchBlendTime;

public:
	AMovementCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
