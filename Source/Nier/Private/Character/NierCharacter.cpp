// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NierCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CombatComponent.h"
#include "Components/StateComponent.h"

ANierCharacter::ANierCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false; // 主要是这个，其他两个默认false
	bUseControllerRotationPitch = false; 

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true; // 使得根运动也能使用转向，但是不用

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void ANierCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(CombatMappingContext, 0);
		}
	}

	StateComponent = GetComponentByClass<UStateComponent>();
}

void ANierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANierCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ANierCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANierCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANierCharacter::OnJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANierCharacter::OnJump);
	}
}

void ANierCharacter::ResetCombat_Implementation()
{
	bIsAttacking = false;
	CombatComp->ResetCombat();
	StateComponent->ResetState();
}

void ANierCharacter::ContinueCombat_Implementation()
{
	StateComponent->ResetState();
}

void ANierCharacter::DisableCombat_Implementation()
{
	bIsCombatEnabled = false;
}

void ANierCharacter::Move(const FInputActionValue& Value)
{
	MoveAxis = Value.Get<FVector2D>();
}

void ANierCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANierCharacter::OnJump(const FInputActionValue& Value)
{
	const bool bJumpped = Value.Get<bool>();

	bJumpped ? Jump() : StopJumping();
}
