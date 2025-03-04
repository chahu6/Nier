// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIER_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetState(FGameplayTag NewState);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentState() const;

	UFUNCTION(BlueprintCallable)
	void ResetState();

protected:
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag CurrentState;
};
