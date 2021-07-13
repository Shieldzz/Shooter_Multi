// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "ShooterGameMode.h"
#include "UndeadCharacter.h"
#include "ShooterCharacter.h"
#include "ShooterGameState.h"

AShooterGameMode::AShooterGameMode() :
	Super(), LevelVictoryName("/Game/Maps/VictoryMap"), GameLevelName("Game/Maps/Highrise")
{
}

void AShooterGameMode::BeginPlay()
{
	ACharacterWithHealth::DeathEvent.Clear();

	ShooterGameState = Cast<AShooterGameState>(GameState);
	if (ShooterGameState == nullptr)
	{
		return;
	}

	KillEventHandle = ACharacterWithHealth::DeathEvent.AddLambda([this](ACharacterWithHealth* charac, ECharacterTeam Team) { AddKill(Team); });
}

void AShooterGameMode::Destroyed()
{
	ACharacterWithHealth::DeathEvent.Remove(KillEventHandle);
}

void AShooterGameMode::Respawn(AController* _playerController)
{
	if (_playerController != nullptr)
	{
		AActor* playerStart = ChoosePlayerStart(_playerController);
		if (playerStart != nullptr)
			RestartPlayerAtPlayerStart(_playerController, playerStart);
	}
	else
		return;
}

void AShooterGameMode::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit);
}

void AShooterGameMode::AddKill(ECharacterTeam Team)
{
	switch (Team)
	{
		case ECharacterTeam::BLUE:
		{
			ShooterGameState->AddKillTeamBlue();
			break;
		}
		case ECharacterTeam::RED:
		{
			ShooterGameState->AddKillTeamRed();
			break;
		}
		case ECharacterTeam::NOTEAM:
		{
			break;
		}
		default:
			break;
	}
}

void AShooterGameMode::SwitchToWinLevel()
{
	ACharacterWithHealth::DeathEvent.Remove(KillEventHandle);
	GetWorld()->ServerTravel(LevelVictoryName);
}

void AShooterGameMode::SwitchToGameLevel()
{
	GetWorld()->ServerTravel(GameLevelName);
}