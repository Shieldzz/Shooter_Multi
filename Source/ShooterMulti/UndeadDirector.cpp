// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "ShooterCharacter.h"
#include "UndeadDirector.h"

AUndeadDirector* AUndeadDirector::Current = nullptr;

// Sets default values
AUndeadDirector::AUndeadDirector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUndeadDirector::BeginPlay()
{
	Super::BeginPlay();

	PunchTimers.Empty();

	Current = this;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AUndeadDirector::SpawnEnemy, SecondPerSpawn, true);
	
	TArray<AActor*> buttons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnerButton::StaticClass(), buttons);

	for (AActor* AButton : buttons)
		SpawnButtons.Add(Cast<ASpawnerButton>(AButton));

	if (SpawnButtons.Num() == 0)
		UE_LOG(LogTemp, Warning, TEXT("Undead Director has no spawn point."));

	//normalize frequencies
	float mag = 0.f;
	
	for (int i = 0; i < SpawnButtons.Num(); ++i)
		mag += SpawnButtons[i]->UseFrequency;
	for (int i = 0; i < SpawnButtons.Num(); ++i)
		SpawnButtons[i]->UseFrequency /= mag;
}

void AUndeadDirector::Destroyed()
{
	AUndeadCharacter::PunchEvent.Remove(PunchEventHandle);
	AUndeadCharacter::DeathEvent.Remove(KillEventHandle);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

	if (Current == this)
		Current = nullptr;
	else
		delete Current;
}

// Called every frame
void AUndeadDirector::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	
	if (Role != ROLE_Authority)
		return;

	//Manual event connection test
	if (Role == ROLE_Authority)
	{
		if (!AUndeadCharacter::DeathEvent.IsBoundToObject(this))
			KillEventHandle = AUndeadCharacter::DeathEvent.AddUObject(this, &AUndeadDirector::OnUndeadKill);
		
		if (!AUndeadCharacter::PunchEvent.IsBoundToObject(this))
			PunchEventHandle = AUndeadCharacter::PunchEvent.AddUObject(this, &AUndeadDirector::OnUndeadPunch);
	}

	for (int i = 0; i < PunchTimers.Num(); ++i)
	{
		PunchTimers[i] += DeltaTime;
		if (PunchTimers[i] >= 1.f)
		PunchTimers.RemoveAt(i--);
	}
}

void AUndeadDirector::SpawnEnemy()
{
	if (SpawnButtons.Num() == 0 || SpawnedCount >= MaxUndeadCount || Role != ROLE_Authority)
		return;

	float rand = FMath::FRand();
	float fq = 0.f;
	
	ASpawnerButton* selectedButton = nullptr;
	for (int i = 0; i < SpawnButtons.Num(); ++i)
	{
		fq += SpawnButtons[i]->UseFrequency;
		if (fq > rand)
		{
			selectedButton = SpawnButtons[i];
			break;
		}
	}

	if (!selectedButton)
		return;

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto undead = GetWorld()->SpawnActor<AUndeadCharacter>(
		selectedButton->UndeadBlueprint,
		selectedButton->GetActorLocation() + selectedButton->GetActorForwardVector(),
		selectedButton->GetActorRotation(),
		spawnParameters);

	undead->SetTeam(selectedButton->Team);

	SpawnedCount++;
}

void AUndeadDirector::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUndeadDirector, PunchTimers);
}

AUndeadDirector* AUndeadDirector::GetCurrent()
{
	return Current;
}

bool AUndeadDirector::PunchAvailable()
{
	return PunchTimers.Num() < MaxPunchPerSecond;
}

void AUndeadDirector::OnUndeadPunch(AUndeadCharacter* instigator)
{
	/*
	if (instigator->Role != ROLE_Authority)
		return;
	*/

	PunchTimers.Add(0.f);
}

void AUndeadDirector::OnUndeadKill(AUndeadCharacter* instigator)
{
	if (instigator->Role != ROLE_Authority)
		return;
	SpawnedCount--;
}
