// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NierCharacterBase.h"

ANierCharacterBase::ANierCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANierCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANierCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANierCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

