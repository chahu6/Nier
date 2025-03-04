// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterRef = GetOwner<ACharacter>();
	if (CharacterRef)
	{
		EquipWeapon();
	}
}

void UCombatComponent::EquipWeapon()
{
	if (BaseWeaponClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = CharacterRef;
		WeaponRef = GetWorld()->SpawnActor<ABaseWeapon>(BaseWeaponClass, Params);
		WeaponRef->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachToSocketName);
	}
}

void UCombatComponent::ResetCombat()
{
	AttackMoveIndex = 0;
	bIsAttacking = false;
	bSaveAttacking = false;
	bCanQuit = true;
	CharacterRef->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void UCombatComponent::ContinueAttack()
{
	if (bSaveAttacking)
	{
		bSaveAttacking = false;
		PerformAttack();
	}
}

void UCombatComponent::KatanaAttack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		CharacterRef->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		WeaponRef->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RootSocketName);

		PerformAttack();
	}
	else
	{
		bSaveAttacking = true;
	}
}

void UCombatComponent::PerformAttack()
{
	if (!IsValid(CharacterRef) || !IsValid(CharacterRef->GetMesh())) return;
	if (AttackMoveIndex < 0) AttackMoveIndex = 0;

	const int32 MontageNum = KatanaMontages.Num();
	AttackMoveIndex = AttackMoveIndex % MontageNum;

	if (UAnimInstance* AnimInstance = CharacterRef->GetMesh()->GetAnimInstance())
	{
		UAnimMontage* MontagePlay = KatanaMontages[AttackMoveIndex];

		BlendingOutDelegate.BindUObject(this, &UCombatComponent::OnMontageBlendingOut);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontagePlay);

		MontageEndedDelegate.BindUObject(this, &UCombatComponent::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontagePlay);

		CharacterRef->StopAnimMontage();
		CharacterRef->PlayAnimMontage(MontagePlay);
	}
	WeaponRef->PlayAttackMontage(AttackMoveIndex);

	++AttackMoveIndex;
}

void UCombatComponent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		//if (!bIsAttacking) Montage_Interrupted();
		bInterruptedCalledBeforeBlendingOut = true;
	}
}

void UCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted && !bInterruptedCalledBeforeBlendingOut)
	{
		//if (!bIsAttacking) Montage_Interrupted();
	}
	else
	{
		//WeaponRef->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
	}
}

void UCombatComponent::Montage_Interrupted()
{
	ResetCombat();
	StopAttackMontage();
}

void UCombatComponent::StopAttackMontage()
{
	CharacterRef->StopAnimMontage();
	WeaponRef->StopAttackMontage();
	WeaponRef->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HandSocketName);
}