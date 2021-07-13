// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterMulti.h"
#include "GameInstanceShooter.h"

UGameInstanceShooter::UGameInstanceShooter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UGameInstanceShooter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameInstanceShooter, Winner);
}