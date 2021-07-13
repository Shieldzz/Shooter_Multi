// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAmmo.generated.h"

UCLASS()
class SHOOTERMULTI_API ASpawnAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnAmmo();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Disapearing, meta = (ClampMin = "0.0"))
	float DisapearingTime = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ammo, meta = (ClampMin = "0.0"))
	int Ammo = 20;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;

	void StartDisapear();
	void UpdateDisapear();
	void FinishDisapear();
	bool IsDisapearing();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool Disapearing;
	float DisapearTimer;
	
};
