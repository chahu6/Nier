// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NierCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "NierCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

class UCombatComponent;
class UStateComponent;


UCLASS()
class NIER_API ANierCharacter : public ANierCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CombatMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

public:
	ANierCharacter();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Player Interface */
	virtual void ResetCombat_Implementation() override;
	virtual void ContinueCombat_Implementation() override;
	virtual void DisableCombat_Implementation() override;
	/** Player Interface end */

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D MoveAxis;

	/** 
	* 目前Test的属性
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bIsAttacking;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bIsCombatEnabled;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComp;

	UPROPERTY()
	TObjectPtr<UStateComponent> StateComponent;

public:
	FORCEINLINE FVector2D GetMoveAxis() const { return MoveAxis; }
};
