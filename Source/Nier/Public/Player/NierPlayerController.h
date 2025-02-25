// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NierPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class NIER_API ANierPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ANierPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
