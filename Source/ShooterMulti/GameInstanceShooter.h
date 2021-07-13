// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CharacterWithHealth.h"
#include "GameInstanceShooter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERMULTI_API UGameInstanceShooter : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameInstanceShooter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Winner")
	ECharacterTeam Winner;

private:
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

};
