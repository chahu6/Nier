// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"

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

void UCombatComponent::KatanaAttack()
{
	if (!IsValid(CharacterRef)) return;

	if (AttackMoveIndex < 0) AttackMoveIndex = 0;

	const int32 MontageNum = KatanaMontages.Num();
	AttackMoveIndex = AttackMoveIndex % MontageNum;

	CharacterRef->StopAnimMontage();
	CharacterRef->PlayAnimMontage(KatanaMontages[AttackMoveIndex]);

	WeaponRef->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RootSocketName);
	WeaponRef->PlayAttackMontage(AttackMoveIndex);

	++AttackMoveIndex;
}
