// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "ShooterGameMode.h"
#include "UndeadAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UndeadCharacter.h"

void AUndeadAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree && BlackboardData)
	{
		UseBlackboard(BlackboardData, BlackboardComponent);

		//APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
		/*
		APawn* player = FindTarget();
		BlackboardComponent->SetValueAsObject(FName("Player"), Cast<ACharacter>(player));
		*/

		RunBehaviorTree(BehaviorTree);
	}
}

void AUndeadAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Role != ROLE_Authority)
		return;

	auto character = Cast<AUndeadCharacter>(GetPawn());

	if (character && character->IsDead())
		Destroy();
	else
	{
		APawn* player = FindTarget();
		BlackboardComponent->SetValueAsObject(FName("Player"), Cast<ACharacter>(player));
	}
}

APawn * AUndeadAIController::FindTarget()
{
	TArray<APlayerController*> players = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode())->PlayerControllers;
	TArray<APawn*> targets;
	AUndeadCharacter* character = Cast<AUndeadCharacter>(GetPawn());
	
	if (!character)
		return nullptr;

	for (APlayerController* p : players)
	{
		AShooterCharacter* player = Cast<AShooterCharacter>(p->GetCharacter());
		if (!player)
			continue;

		if (player->Team != character->Team && !player->IsDead())
			targets.Add(p->GetPawn());
	}

	if (targets.Num() == 0)
		return nullptr;

	if (targets.Num() == 1)
		return targets[0];

	return GetNearestFromCharacter(targets);
}

bool AUndeadAIController::Punch()
{
	auto character = Cast<AUndeadCharacter>(GetPawn());

	if (character)
		return character->Punch();

	return false;
}

APawn * AUndeadAIController::GetNearestFromCharacter(TArray<APawn*> Pawns)
{
	if (!GetCharacter() || Pawns.Num() == 0)
		return nullptr;

	FVector pos = GetCharacter()->ActorToWorld().GetTranslation();
	float distance = FVector::Dist(pos, Pawns[0]->ActorToWorld().GetTranslation());
	float curdistance = 0.f;
	APawn* nearest = Pawns[0];

	for (APawn* p : Pawns)
	{
		curdistance = FVector::Dist(p->ActorToWorld().GetTranslation(), pos);
		if (curdistance <= distance)
		{
			distance = curdistance;
			nearest = p;
		}
	}
	return nearest;
}
