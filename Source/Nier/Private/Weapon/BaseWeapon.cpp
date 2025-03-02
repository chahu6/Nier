// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::PlayAttackMontage(int32 InAttackMoveIndex)
{
	if (KatanaMontages.IsEmpty()) return;

	const int32 AttackMoveIndex = InAttackMoveIndex % KatanaMontages.Num();
	if (UAnimInstance* Instance = MeshComp->GetAnimInstance())
	{
		Instance->Montage_Play(KatanaMontages[AttackMoveIndex]);
	}
}
