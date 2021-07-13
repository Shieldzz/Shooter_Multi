// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "SpawnAmmo.h"
#include "ShooterCharacter.h"


// Sets default values
ASpawnAmmo::ASpawnAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnAmmo::BeginPlay()
{
	Super::BeginPlay();
	


}

// Called every frame
void ASpawnAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Disapearing)
	{
		DisapearTimer += DeltaTime;
		UpdateDisapear();
	}
}

void ASpawnAmmo::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (Role == ROLE_Authority)
	{
		AShooterCharacter* player = Cast<AShooterCharacter>(OtherActor);
		if (player != nullptr && player->CurrentBackpackAmmo < player->MaxBackpackAmmo)
		{
			StartDisapear();
			player->AddAmmo();
		}
	}
}

void ASpawnAmmo::StartDisapear()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	Disapearing = true;
	DisapearTimer = 0.0f;
}

void ASpawnAmmo::UpdateDisapear()
{
	if (DisapearTimer > DisapearingTime)
		return FinishDisapear();
}

void ASpawnAmmo::FinishDisapear()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	Disapearing = false;
}

bool ASpawnAmmo::IsDisapearing()
{
	return Disapearing;
}