// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyBohemianLifeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BohemianMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_MAX			UMETA(Hidden),
};

UCLASS()
class MYBOHEMIANLIFE_API UBohemianMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Bohemian : public FSavedMove_Character
	{
	public:
		
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Custom_1		= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};
		
		// Flag
		uint8 Saved_bWantsToSprint:1;
		
		uint8 Saved_bPrevWantsToCrouch:1;
		uint8 Saved_bWantsToProne:1;

		FSavedMove_Bohemian();
		
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Bohemian : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Bohemian(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	/** Parameters */

	// // Sprint property
	// UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	// UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;
	//
	// // Slide property
	// UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed = 350;
	// UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse = 500;
	// UPROPERTY(EditDefaultsOnly) float Slide_GravityForce = 5000;
	// UPROPERTY(EditDefaultsOnly) float Slide_Friction = 1.3;
	
	UPROPERTY(EditDefaultsOnly) float MaxSprintSpeed=1000.f;

	UPROPERTY(EditDefaultsOnly) float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly) float MaxSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse=400.f;
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly) float SlideFrictionFactor=.06f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding=1000.f;

	UPROPERTY(EditDefaultsOnly) float Prone_EnterHoldDuration=.2f;
	UPROPERTY(EditDefaultsOnly) float ProneSlideEnterImpulse=300.f;
	UPROPERTY(EditDefaultsOnly) float MaxProneSpeed=300.f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationProning=2500.f;

	
	/** Transient */
	UPROPERTY(Transient) AMyBohemianLifeCharacter* BohemianLifeCharacter;
	
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;
	FTimerHandle TimerHandle_EnterProne;
	

	
public:
	UBohemianMovementComponent();
	
protected:
	virtual void InitializeComponent() override;

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	
public:
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
private:
	/** Slide */
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	bool CanSlide() const;
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

	/** Prone */
	void TryEnterProne() { Safe_bWantsToProne = true; }
	
	UFUNCTION(Server, Reliable)
	void Server_EnterProne();
	
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iterations);
	
public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();
	
	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const; 
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;
};
