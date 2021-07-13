// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "ShooterGameState.h"
#include "UndeadCharacter.h"
#include "ShooterCharacter.h"
#include "ShooterGameMode.h"
#include "GameInstanceShooter.h"
#include "CharacterWithHealth.h"
#include "BShooterMultiPlayerState.h"

AShooterGameState::AShooterGameState()
	: Super()
{

}

void AShooterGameState::BeginPlay()
{
	GameMode = Cast<AShooterGameMode>(AuthorityGameMode);
	GameInstance = Cast<UGameInstanceShooter>(GetGameInstance());
}

void AShooterGameState::Destroyed()
{
}

int	AShooterGameState::GetScoreTeamBlue()
{
	return ScoreTeamBlue;
}

int	AShooterGameState::GetScoreTeamRed()
{
	return ScoreTeamRed;
}

int	AShooterGameState::GetScoreForVictory()
{
	return ScoreForVictory;
}

void AShooterGameState::AddKillTeamBlue()
{
	if (Role == ROLE_Authority)
	{
		++ScoreTeamBlue;
		if (ScoreTeamBlue == ScoreForVictory)
		{
			RPCMulticast_SetWinner(ECharacterTeam::BLUE);
			GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &AShooterGameMode::SwitchToWinLevel, 5.0f, false);
		}
	}
}

void AShooterGameState::AddKillTeamRed()
{
	if (Role == ROLE_Authority)
	{
		++ScoreTeamRed;
		if (ScoreTeamRed == ScoreForVictory)
		{
			RPCMulticast_SetWinner(ECharacterTeam::RED);
			GetWorldTimerManager().SetTimer(UnusedHandle, GameMode, &AShooterGameMode::SwitchToWinLevel, 0.5f, false);
		}
	}
}

void AShooterGameState::ResetGame()
{
	ScoreTeamRed = 0;
	ScoreTeamBlue = 0;
}

void AShooterGameState::RPCMulticast_SetWinner_Implementation(ECharacterTeam _team)
{
	GameInstance->Winner = _team;
}

bool AShooterGameState::RPCMulticast_SetWinner_Validate(ECharacterTeam _team)
{
	return true;
}

void AShooterGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterGameState, ScoreTeamBlue);
	DOREPLIFETIME(AShooterGameState, ScoreTeamRed);
}