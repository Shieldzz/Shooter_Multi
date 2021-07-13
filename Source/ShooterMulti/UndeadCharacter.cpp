// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "UndeadCharacter.h"
#include "CharacterWithHealth.h"
#include "HitDamage.h"

AUndeadCharacter::FUndeadEvent AUndeadCharacter::PunchEvent;
AUndeadCharacter::FUndeadEvent AUndeadCharacter::DeathEvent;

// Sets default values
AUndeadCharacter::AUndeadCharacter() :
	Super()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsPlayerFriendly = false;

	//Create Punch Collision
	PunchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PunchCollision"));
	PunchCollision->SetupAttachment(RootComponent);
	PunchCollision->SetRelativeLocation(FVector(80.f, 0.f, 20.f));
	PunchCollision->InitSphereRadius(20.f);
}

// Called when the game starts or when spawned
void AUndeadCharacter::BeginPlay()
{
	Super::BeginPlay();
	punchTimer = 0.f;
	hasPunched = false;
}

// Called every frame
void AUndeadCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	hasPunched = false;
	punchTimer -= DeltaTime;
}

bool AUndeadCharacter::Punch()
{
	if (punchTimer > 0.f ||hasPunched)
		return false;

	hasPunched = true;
	if (Role != ROLE_Authority)
		SendPunchState(hasPunched);
	
	punchTimer = PunchCooldown;

	PunchEvent.Broadcast(this);

	return true;
}

bool AUndeadCharacter::GetHasPunched()
{
	return hasPunched;
}

void AUndeadCharacter::InflictPunch()
{
	TArray<struct FHitResult> outHits;

	FVector pos = PunchCollision->GetComponentLocation();

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;

	GetWorld()->SweepMultiByObjectType(
		outHits, GetActorLocation(), pos, FQuat::Identity, 0,
		FCollisionShape::MakeSphere(PunchCollision->GetUnscaledSphereRadius()),
		params);

	TArray<ACharacterWithHealth*> hitActors;

	for (auto& hit : outHits)
	{
		ACharacterWithHealth* character = Cast<ACharacterWithHealth>(hit.Actor.Get());

		if (character && character->IsPlayerFriendly != IsPlayerFriendly && !hitActors.Contains(character))
		{
			if (!hitActors.Contains(character))
			{
				FPointDamageEvent damageEvent = FPointDamageEvent(Damages, hit, GetActorForwardVector(), TSubclassOf<UDamageType>(UHitDamage::StaticClass()));
				character->TakeDamage(Damages, damageEvent, nullptr, this);
				hitActors.Add(character);
			}

			//push hit actors (physics)
			for (auto aSkeletalMesh : character->GetComponentsByClass(USkeletalMeshComponent::StaticClass()))
			{
				USkeletalMeshComponent* skeletalMesh = Cast<USkeletalMeshComponent>(aSkeletalMesh);
				if (skeletalMesh->IsSimulatingPhysics())
					skeletalMesh->AddForceAtLocation(-hit.ImpactNormal * PunchKnockback, hit.ImpactPoint, hit.BoneName);
			}
		}
	}
}

void AUndeadCharacter::Die()
{
	Super::Die();
	if (Role == ROLE_Authority)
		DeathEvent.Broadcast(this);
}

void AUndeadCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUndeadCharacter, hasPunched);
}

void AUndeadCharacter::SendPunchState_Implementation(bool State)
{
	hasPunched = State;
}

bool AUndeadCharacter::SendPunchState_Validate(bool State)
{
	return true;
}
