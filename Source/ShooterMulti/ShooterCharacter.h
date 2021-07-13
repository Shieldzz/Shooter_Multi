// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "net/UnrealNetwork.h"
#include "CharacterWithHealth.h"

class USoundBase;
class UCameraShake;

#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class EShooterCharacterState : uint8
{
	IdleRun,
	Aim,
	Sprint,
	Reload,
	Jump,
	Punch,
	Dead
};

UCLASS()
class SHOOTERMULTI_API AShooterCharacter : public ACharacterWithHealth
{
	GENERATED_BODY()

public:

#pragma region Members

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter)
	float SprintSpeed = 1000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter)
	float AimWalkSpeed = 180.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter)
	float ReloadWalkSpeed = 200.f;

	UPROPERTY(Transient, BlueprintReadOnly)
	float RunSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MinSprintMagnitude = .3f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter, meta = (ClampMin = "0.0"))
	float CameraAimDistance = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter, meta = (ClampMin = "0.0"))
	float CameraAimFOV = 75.f;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	float CameraRestDistance;

	UPROPERTY(Transient, BlueprintReadOnly)
	float CameraRestFOV;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter, meta = (ClampMin = "0"))
	float PunchDuration = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter)
	TSubclassOf<UCameraShake> RunShake;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ShooterCharacter)
	TSubclassOf<UCameraShake> SprintShake;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon", meta = (ClampMin = "0"))
	int MaxAmmo = 20;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon", meta = (ClampMin = "0"))
	int CurrentBackpackAmmo = 20;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon", meta = (ClampMin = "0"))
	int MaxBackpackAmmo = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon", meta = (ClampMin = "0"))
	int AddBackpackAmmo = 20;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon", meta = (ClampMin = "0"))
	float FireRate = .24f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	USoundBase* ShotSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	USoundBase* ShotEmptySound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	UMaterial* ImpactDecalMat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float ImpactDecalLifeSpan = 30.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float ImpactDecalSize = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	class UParticleSystem* ImpactParticle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	class UParticleSystem* BeamParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float BeamIntensity = 5000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	UCurveFloat* BeamIntensityCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	class UParticleSystem* MuzzleSmokeParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponDamages = 15.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponPunchDamages = 30.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponKnokback = 300000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponMinSpreadAim = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponMinSpreadWalk = 4.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponMaxSpread = 15.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponSpreadPerShot = 4.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	float WeaponSpreadRecoveryRate = 1.5f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "ShooterCharacter|Weapon")
	float CurrentSpread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Invisble, meta = (ClampMin = "0.0"))
	float InvisibleTime = 5.0f;

	UPROPERTY(Replicated)
	FRotator	RepRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShooterCharacter|Weapon")
	TSubclassOf<UCameraShake> ShootShake;

	//|ANCHOR|
	UPROPERTY(Transient, BlueprintReadOnly, Replicated)
	EShooterCharacterState CurrentState = EShooterCharacterState::IdleRun;

	UPROPERTY(Category = UndeadCharacter, VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* PunchCollision;

	DECLARE_EVENT_OneParam(ACharacterWithHealth, FShooterEvent, AShooterCharacter*)
	static FShooterEvent DeathEvent;

#pragma endregion

	// --- ----- --- //

#pragma region Unreal Basics

	// Sets default values for this character's properties
	AShooterCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

#pragma endregion

	// --- ----- --- //

#pragma region Controller

	void Turn(float Value);
	void LookUp(float Value);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartJump();
	void EndJump();

	void StartSprint();
	void EndSprint();

	void StartAim();
	void EndAim();

#pragma endregion

	UFUNCTION(BlueprintPure, Category = "Shooter|Character")
	bool IsShooting();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void StartShoot();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void EndShoot();

	UFUNCTION(BlueprintPure, Category = "Shooter|Character")
	bool IsInvisible();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void StartInvisible();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void UpdateInvisible();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void EndInvisible();

	UFUNCTION(BlueprintPure, Category = "Shooter|Character")
	int GetAmmo();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void Reload();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void EndReload();
	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void AbortReload();

	UFUNCTION(BlueprintCallable, Category = "Shooter|Character")
	void Punch();

	UFUNCTION(BlueprintCallable, Category = Character)
	void InflictPunch();

	void TakeShot();

	void UpdateAmmo();
	
	UFUNCTION(Reliable, Server, WithValidation)
		void AddAmmo();
	void AddAmmo_Implementation();
	bool AddAmmo_Validate();

	UFUNCTION(BlueprintCallable, Category = Health)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	virtual void Die() override;

	virtual void FinishDisapear() override;

	bool ConsumeHitTrigger();
	
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &
		OutLifetimeProps) const override;

private:
	bool CanSprint;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	float TargetSpringArmLength;
	float TargetCameraFOV;

	UPROPERTY(Replicated)
	int Ammo;

	UPROPERTY(Replicated)
	bool Shooting;
	float ShootTimer;
	float StateTimer;

	UPROPERTY(Replicated)
	bool GotHit;
	
	UPROPERTY(ReplicatedUsing = OnRep_InvBool)
	bool Invisible;
	float InvisibleTimer;

	//|ANCHOR|

#pragma region RPC

	UFUNCTION(Reliable, Server, WithValidation)
	void	SendAnimState(EShooterCharacterState State);
	void	SendAnimState_Implementation(EShooterCharacterState State);
	bool	SendAnimState_Validate(EShooterCharacterState State);

	UFUNCTION(Reliable, Server, WithValidation)
	void	SendShootingState(bool ShootingB);
	void	SendShootingState_Implementation(bool ShootingB);
	bool	SendShootingState_Validate(bool ShootingB);

	UFUNCTION(Reliable, Server, WithValidation)
	void	SendHitState(bool HitB);
	void	SendHitState_Implementation(bool HitB);
	bool	SendHitState_Validate(bool HitB);

	UFUNCTION(Reliable, Server, WithValidation)
	void	SendRotation(FRotator Rot);
	void	SendRotation_Implementation(FRotator Rot);
	bool	SendRotation_Validate(FRotator Rot);

#pragma endregion

	void	CheckAnimState();

	UFUNCTION()
		void OnRep_InvBool();

	EShooterCharacterState		ServerState;
};
