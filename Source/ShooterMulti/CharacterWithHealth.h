// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "CharacterWithHealth.generated.h"

UENUM(BlueprintType)
enum class ECharacterTeam : uint8
{
	BLUE = 0	UMETA(DisplayName = "Blue"),
	RED			UMETA(DisplayName = "Red"),
	NOTEAM		UMETA(DisplayName = "NoTeam"),
	TEAM
};

UCLASS()
class SHOOTERMULTI_API ACharacterWithHealth : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health, meta = (ClampMin = "0.0"))
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health, meta = (ClampMin = "0.0"))
	bool IsPlayerFriendly = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health, meta = (ClampMin = "0.0"))
	float TimeBeforeDisapearing = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health, meta = (ClampMin = "0.0"))
	float DisapearingTime = 10.f;

	UPROPERTY(ReplicatedUsing = OnRep_Team, BlueprintReadWrite, EditAnywhere, Category = Team)
		ECharacterTeam Team;

	// Sets default values for this character's properties
	ACharacterWithHealth();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	DECLARE_EVENT_TwoParams(ACharacterWithHealth, FUndeadEvent, ACharacterWithHealth*, ECharacterTeam)
	static FUndeadEvent DeathEvent;


	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealth();
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual float GainHealth(float GainAmount);
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void ResetHealth();

	UFUNCTION(BlueprintPure, Category = Health)
	bool IsDead();
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void StartDisapear();
	UFUNCTION()
	void UpdateDisapear();
	UFUNCTION(BlueprintCallable, Category = Health)
	virtual void FinishDisapear();
	UFUNCTION(BlueprintPure, Category = Health)
	FORCEINLINE bool IsDisapearing() { return Disapearing; }

	UFUNCTION(BlueprintCallable, Category = "Team")
		void SetTeam(ECharacterTeam _team);

	UFUNCTION(BlueprintCallable, Category = "Team")
		ECharacterTeam	GetTeam();
	
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void RPCServer_UpdateTeam(ECharacterTeam _team);

	void RPCServer_UpdateTeam_Implementation(ECharacterTeam _team);
	bool RPCServer_UpdateTeam_Validate(ECharacterTeam _team);

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &OutLifetimeProps) const override;

private:
	
	void RetrieveMaterials();

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;

	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION()
	void OnRep_Team();

	bool Disapearing;
	float DisapearTimer;

	TArray<UMaterialInstanceDynamic*> DissolveMaterials;
};
