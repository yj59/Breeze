// Fill out your copyright notice in the Description page of Project Settings.


#include "Comps/BohemianMovementComponent.h"

#include "GameFramework/Character.h"

#pragma region Saved Move

// UBohemianMovementComponent::FSavedMove_Bohemian::FSavedMove_Bohemian()
// {
// 	Saved_bWantsToSprint=0;
// }

bool UBohemianMovementComponent::FSavedMove_Bohemian::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Bohemian* NewBohemianMove = static_cast<FSavedMove_Bohemian*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewBohemianMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UBohemianMovementComponent::FSavedMove_Bohemian::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint=0;
}

uint8 UBohemianMovementComponent::FSavedMove_Bohemian::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if(Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	return Result;
}

void UBohemianMovementComponent::FSavedMove_Bohemian::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UBohemianMovementComponent* CharacterMovement = Cast<UBohemianMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UBohemianMovementComponent::FSavedMove_Bohemian::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UBohemianMovementComponent* CharacterMovement = Cast<UBohemianMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

#pragma endregion

#pragma region Client Network Prediction Data

UBohemianMovementComponent::FNetworkPredictionData_Client_Bohemian::FNetworkPredictionData_Client_Bohemian(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{
}

FSavedMovePtr UBohemianMovementComponent::FNetworkPredictionData_Client_Bohemian::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Bohemian());
}

#pragma endregion

#pragma region CMC

UBohemianMovementComponent::UBohemianMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

FNetworkPredictionData_Client* UBohemianMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UBohemianMovementComponent* MutableThis = const_cast<UBohemianMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Bohemian(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f; 
	}
	return ClientPredictionData;
}

void UBohemianMovementComponent::OnClientCorrectionReceived(FNetworkPredictionData_Client_Character& ClientData,
	float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName,
	bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode)
{
	Super::OnClientCorrectionReceived(ClientData, TimeStamp, NewLocation, NewVelocity, NewBase, NewBaseBoneName, bHasBase, bBaseRelativePosition, ServerMovementMode);
}

void UBohemianMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Bohemian::FLAG_Custom_0) != 0;
}

void UBohemianMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
			// UE_LOG(LogTemp, Warning, TEXT("SprintPressed: %f"), Sprint_MaxWalkSpeed);
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
			// UE_LOG(LogTemp, Warning, TEXT("SprintReleased: %f"), Walk_MaxWalkSpeed);
		}
	}
}

#pragma endregion

void UBohemianMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UBohemianMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UBohemianMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}




