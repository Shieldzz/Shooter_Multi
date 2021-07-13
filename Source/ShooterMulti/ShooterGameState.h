// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "BShooterMultiPlayerState.h"

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameState.generated.h"

class AShooterGameMode;
class UGameInstanceShooter;
/**
 * 
 */
UCLASS()
class SHOOTERMULTI_API AShooterGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AShooterGameState();

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = Score)
		int ScoreForVictory;

	UFUNCTION(BlueprintCallable, Category = "GameState")
		int	GetScoreTeamBlue();

	UFUNCTION(BlueprintCallable, Category = "GameState")
		int	GetScoreTeamRed();

	UFUNCTION(BlueprintCallable, Category = "GameState")
		int	GetScoreForVictory();

	void AddKillTeamBlue();
	void AddKillTeamRed();

	void ResetGame();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void RPCMulticast_SetWinner(ECharacterTeam _team);

	void RPCMulticast_SetWinner_Implementation(ECharacterTeam _team);
	bool RPCMulticast_SetWinner_Validate(ECharacterTeam _team);


private:
	
	UPROPERTY(Replicated)
		int ScoreTeamRed;
	UPROPERTY(Replicated)
		int ScoreTeamBlue;
	
	
	AShooterGameMode* GameMode;
	UGameInstanceShooter* GameInstance;

	FTimerHandle UnusedHandle;

private:

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;
};
