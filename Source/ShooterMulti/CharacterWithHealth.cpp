// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "CharacterWithHealth.h"
#include "BShooterMultiPlayerState.h"

ACharacterWithHealth::FUndeadEvent ACharacterWithHealth::DeathEvent;

// Sets default values
ACharacterWithHealth::ACharacterWithHealth()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void ACharacterWithHealth::BeginPlay()
{
	Super::BeginPlay();

	Disapearing = false;
	ResetHealth();

	if (DissolveMaterials.Num() == 0)
		RetrieveMaterials();

	for (auto mat : DissolveMaterials)
	{
		mat->SetScalarParameterValue(FName(TEXT("DissolveAmmount")), 0.f);
	}
}

void ACharacterWithHealth::Tick(float DeltaTime)
{
	if (IsDead())
	{
		DisapearTimer += DeltaTime;

		if (!Disapearing && DisapearTimer > TimeBeforeDisapearing)
			StartDisapear();

		if (Disapearing)
			UpdateDisapear();
	}
}

float ACharacterWithHealth::GetHealth()
{
	return Health;
}

float ACharacterWithHealth::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float doneDamages = 0.f;

	const FPointDamageEvent* pointDamage = nullptr;
	ACharacterWithHealth* playerCauser = Cast<ACharacterWithHealth>(DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		pointDamage = (FPointDamageEvent*)&DamageEvent;

	if (!IsDead() && playerCauser != nullptr)
	{
		if (pointDamage && pointDamage->HitInfo.PhysMaterial.Get())
			doneDamages = DamageAmount * pointDamage->HitInfo.PhysMaterial->DestructibleDamageThresholdScale;
		else
			doneDamages = DamageAmount;

		if (Team != playerCauser->GetTeam() && Role == ROLE_Authority)
		{
			Health = FMath::Max(0.f, Health - doneDamages);
		}

		if (Health <= 0.f)
		{
			if (Role == ROLE_Authority)
			{
				if (GetTeam() != ECharacterTeam::NOTEAM)
					DeathEvent.Broadcast(this, playerCauser->GetTeam());
				Die();
			}

		}
	}

	return doneDamages;
}

float ACharacterWithHealth::GainHealth(float GainAmount)
{
	if (!IsDead())
	{
		if (Health + GainAmount > MaxHealth)
		{
			GainAmount = MaxHealth - Health;
			Health = MaxHealth;
		}
		else
		{
			Health = FMath::Min(MaxHealth, Health + GainAmount);
		}

		return GainAmount;
	}

	return 0.f;
}

void ACharacterWithHealth::ResetHealth()
{
	Health = MaxHealth;
}

bool ACharacterWithHealth::IsDead()
{
	return Health <= 0.f;
}

void ACharacterWithHealth::Die()
{
	Health = 0.f;

	auto capsule = GetCapsuleComponent();
	capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	auto mesh = GetMesh();

	for (auto aSkeletalMesh : GetComponentsByClass(USkeletalMeshComponent::StaticClass()))
	{
		USkeletalMeshComponent* skeletalMesh = Cast<USkeletalMeshComponent>(aSkeletalMesh);
		skeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		skeletalMesh->bPauseAnims = true;
		skeletalMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
		skeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);
		skeletalMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		skeletalMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		skeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		skeletalMesh->SetSimulatePhysics(true);
	}
}

void ACharacterWithHealth::StartDisapear()
{
	DisapearTimer = 0.f;
	Disapearing = true;
}

void ACharacterWithHealth::UpdateDisapear()
{
	if (DisapearTimer > DisapearingTime)
		return FinishDisapear();

	GetMesh()->SetScalarParameterValueOnMaterials(FName(TEXT("DissolveAmmount")), DisapearTimer / DisapearingTime);
}

void ACharacterWithHealth::FinishDisapear()
{
	Destroy();
}

void ACharacterWithHealth::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterWithHealth, Health);
	DOREPLIFETIME(ACharacterWithHealth, Team);
}

void ACharacterWithHealth::RetrieveMaterials()
{
	for (auto uskelMesh : GetComponentsByClass(USkeletalMeshComponent::StaticClass()))
	{
		auto skelMesh = Cast<USkeletalMeshComponent>(uskelMesh);
		auto mats = skelMesh->GetMaterials();

		for (int i = 0; i < mats.Num(); ++i)
		{
			auto newMat = UMaterialInstanceDynamic::Create(mats[i], this);
			skelMesh->SetMaterial(i, newMat);
			DissolveMaterials.Add(newMat);
		}
	}

	for (auto ustaticMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
	{
		auto staticMesh = Cast<UStaticMeshComponent>(ustaticMesh);
		auto mats = staticMesh->GetMaterials();

		for (int i = 0; i < mats.Num(); ++i)
		{
			auto newMat = UMaterialInstanceDynamic::Create(mats[i]->GetMaterial(), this);
			staticMesh->SetMaterial(i, newMat);
			DissolveMaterials.Add(newMat);
		}
	}
}

void ACharacterWithHealth::OnRep_Health()
{
	if (Health <= 0)
		Die();
}

void ACharacterWithHealth::OnRep_Team()
{
	SetTeam(Team);
}

void ACharacterWithHealth::SetTeam(ECharacterTeam _team)
{

	Team = _team;
	FLinearColor color = FLinearColor::White;
	
	switch (Team)
	{
	case ECharacterTeam::BLUE:
		color = FLinearColor::Blue;
		break;
	case ECharacterTeam::RED:
		color = FLinearColor::Red;
		break;
	default:
		break;
	}

	if (DissolveMaterials.Num() == 0)
		RetrieveMaterials();

	for (auto mat : DissolveMaterials)
		mat->SetVectorParameterValue(FName(TEXT("BodyColor")), color);
}

ECharacterTeam	ACharacterWithHealth::GetTeam()
{
	return Team;
}

void ACharacterWithHealth::RPCServer_UpdateTeam_Implementation(ECharacterTeam _team)
{
	SetTeam(_team);
}

bool ACharacterWithHealth::RPCServer_UpdateTeam_Validate(ECharacterTeam _team)
{
	if (_team == ECharacterTeam::TEAM)
		return false;

	return true;
}