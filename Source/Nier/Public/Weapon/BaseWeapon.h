// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class NIER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void PlayAttackMontage(int32 InAttackMoveIndex);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComp;

protected:
	/**
	* Montage Setting
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> KatanaMontages;
};
