// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterWithHealth.h"
#include "SpawnerButton.generated.h"

UCLASS()
class SHOOTERMULTI_API ASpawnerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerButton();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool	IsAvailable();

	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	TSubclassOf<class AUndeadCharacter> UndeadBlueprint;
	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	float UseFrequency;
	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	float ResetTimer = 5.f;
	UPROPERTY(EditAnywhere, BlueprintInternalUseOnly)
	ECharacterTeam Team = ECharacterTeam::NOTEAM;

	//void	SetSpawnpoint(FUndeadSpawnPoint _point);
	void	Trigger(ACharacterWithHealth* _instigator);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CurrentTimer = 0.f;
};
