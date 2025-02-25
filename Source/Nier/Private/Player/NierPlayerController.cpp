// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NierPlayerController.h"
#include "GameFramework/Character.h"

ANierPlayerController::ANierPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANierPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ANierPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void ANierPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	
}