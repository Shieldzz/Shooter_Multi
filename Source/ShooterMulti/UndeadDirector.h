// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "SpawnerButton.h"
#include "UndeadCharacter.h"
#include "UndeadDirector.generated.h"

USTRUCT()
struct FUndeadSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	TSubclassOf<class AUndeadCharacter> UndeadBlueprint;
	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	float UseFrequency;
	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	AActor* Point;
};

UCLASS()
class SHOOTERMULTI_API AUndeadDirector : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintInternalUseOnly, Category = Director)
	TArray<ASpawnerButton*> SpawnButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Director)
	int MaxPunchPerSecond = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Director)
	int MaxUndeadCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Director, meta=(ClampMin = 0.1f))
	float SecondPerSpawn = 5.f;

	// Sets default values for this actor's properties
	AUndeadDirector();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UFUNCTION(BlueprintPure, Category = Director)
	static AUndeadDirector* GetCurrent();

	UFUNCTION(BlueprintPure, Category = Director)
	bool PunchAvailable();

	void OnUndeadPunch(AUndeadCharacter* instigator);
	void OnUndeadKill(AUndeadCharacter* instigator);

	void SpawnEnemy();

protected:

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > &
		OutLifetimeProps) const override;

	int		SpawnedCount = 0;

	static AUndeadDirector* Current;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(Replicated)
	TArray<float> PunchTimers;

	FDelegateHandle PunchEventHandle;
	FDelegateHandle KillEventHandle;
};
