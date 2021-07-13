// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "SpawnerButton.h"


// Sets default values
ASpawnerButton::ASpawnerButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Team != ECharacterTeam::NOTEAM)
	{
		CurrentTimer += DeltaTime;
		if (CurrentTimer >= ResetTimer)
		{
			Team = ECharacterTeam::NOTEAM;
			CurrentTimer = 0.f;
		}
	}
}

bool ASpawnerButton::IsAvailable()
{
	if (Team == ECharacterTeam::NOTEAM)
		return true;
	else
		return false;
}

void ASpawnerButton::Trigger(ACharacterWithHealth * _instigator)
{
		Team = _instigator->GetTeam();
}

