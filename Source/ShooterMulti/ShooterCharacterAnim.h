// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "ShooterCharacter.h"
#include "ShooterCharacterAnim.generated.h"

/**
 * 
 */
UCLASS(Transient, Blueprintable)
class SHOOTERMULTI_API UShooterCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	FVector2D Direction;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	EShooterCharacterState State;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	float RelativeSpeed;
	
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	float AimYaw;

	UPROPERTY(Transient, BlueprintReadOnly)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources|Infos")
	float FireAnimLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources|Montages")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources|Montages")
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources|Montages")
	UAnimMontage* FireAimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources|Montages")
	UAnimMontage* PunchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources|Montages")
	UAnimMontage* HitMontage;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_PunchHit(UAnimNotify* Notify);

/*	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &
		OutLifetimeProps) const override;
		*/
protected:

/*	UFUNCTION(Reliable, Server, WithValidation)
		void SendRotation(FRotator _rot);

	void SendRotation_Implementation(FRotator _rot);
	bool SendRotation_Validate(FRotator _rot);
	
	UPROPERTY(Replicated)
	FRotator	rotation;
	*/
	AShooterCharacter* ShooterCharacter;
	bool PunchMontagePlayed = false;

};
