// Fill out your copyright notice in the Description page of Project Settings.


#include "Basic/MovementCameraManager.h"

#include "Character/MyBohemianLifeCharacter.h"
#include "Comps/BohemianMovementComponent.h"
#include "Components/CapsuleComponent.h"


AMovementCameraManager::AMovementCameraManager()
{

}

void AMovementCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
	
	if (AMyBohemianLifeCharacter* BohemianCharacter = Cast<AMyBohemianLifeCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UBohemianMovementComponent* BCMC = BohemianCharacter->GetBohemianMovement();
		
		FVector TargetCrouchOffset = FVector(
			0,
			0,
			BCMC->GetCrouchedHalfHeight() - BohemianCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (BCMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
