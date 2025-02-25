// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NierAnimInstance.h"
#include "Character/NierCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NierCharacter = Cast<ANierCharacter>(TryGetPawnOwner());
}

void UNierAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	NierCharacter = NierCharacter == nullptr ? Cast<ANierCharacter>(TryGetPawnOwner()) : NierCharacter;

	if (NierCharacter == nullptr) return;

	Speed = NierCharacter->GetVelocity().Size2D();

	MoveAxis = NierCharacter->GetMoveAxis();

	bIsInAir = NierCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = NierCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

	bIsCrouched = NierCharacter->GetCharacterMovement()->IsCrouching();
}
