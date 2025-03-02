// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ACharacter;
class ABaseWeapon;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NIER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	void EquipWeapon();

	void ResetCombat();

	UFUNCTION(BlueprintCallable)
	void KatanaAttack();
		
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> CharacterRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseWeapon> BaseWeaponClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABaseWeapon> WeaponRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachToSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RootSocketName;

	/**
	* Montage Setting
	*/

	UPROPERTY(BlueprintReadWrite, Category = "Montage")
	int32 AttackMoveIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<UAnimMontage*> KatanaMontages;
};
