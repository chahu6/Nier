// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StateComponent.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStateComponent::SetState(FGameplayTag NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
	}
}

FGameplayTag UStateComponent::GetCurrentState() const
{
	return CurrentState;
}

void UStateComponent::ResetState()
{
	SetState(FGameplayTag());
}
