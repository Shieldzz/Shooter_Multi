// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "ShooterCharacter.h"
#include "ShooterGameMode.generated.h"

class AShooterGameState;

/**
 * 
 */
UCLASS()
class SHOOTERMULTI_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AShooterGameMode();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gamemode")
	TArray<APlayerController*>	PlayerControllers;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void AddKill(ECharacterTeam Team);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void Respawn(AController* controller);

	void LaunchEndMenu();

	void SwitchToWinLevel();
	void SwitchToGameLevel();

	void Quit();

private:

	AShooterGameState* ShooterGameState;
	
	const FString LevelVictoryName;
	const FString GameLevelName;

	FDelegateHandle KillEventHandle;
	FDelegateHandle DeathEventHandle;
};
