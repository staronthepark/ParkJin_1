// Fill out your copyright notice in the Description page of Project Settings.


#include "../Controller/MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMyPlayerController::BeginPlay()
{
	character = Cast<AMyProjectCharacter>(GetPawn());

	PlayerState = character->GetCurStateType();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	character->Look(Value);
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	character->Move(Value);
}

void AMyPlayerController::PressMove()
{
	if (character->IsActiveCanclebyMove()) {
		if (character->GetAnimInstance()->IsAnyMontagePlaying()) {
		character->StopPlayerMontage();
		}
	}
}

void AMyPlayerController::MoveEnd()
{
	character->MoveEnd();
}

void AMyPlayerController::Dodge()
{
	character->CallInputFunc(*PlayerState, EInputType::DODGE, true);
}

void AMyPlayerController::PressSprint()
{
	character->CallInputFunc(*PlayerState, EInputType::SPRINT, true);
}

void AMyPlayerController::UnPressSprint()
{
	character->CallInputFunc(*PlayerState, EInputType::SPRINT, false);
}

void AMyPlayerController::PressShield()
{
	character->CallInputFunc(*PlayerState, EInputType::SHIELD, true);
}

void AMyPlayerController::UnPressShield()
{
	character->CallInputFunc(*PlayerState, EInputType::SHIELD, false);
}

void AMyPlayerController::PressHeal()
{
	character->CallInputFunc(*PlayerState, EInputType::HEAL, true);
}

void AMyPlayerController::PressAttack()
{
	character->CallInputFunc(*PlayerState, EInputType::ATTACK, true);
}

void AMyPlayerController::PressInteraction()
{
	character->CallInputFunc(*PlayerState, EInputType::INTERACTION, true);
}

void AMyPlayerController::PressQuit()
{
	character->CallInputFunc(EPlayerState::NONE, EInputType::QUIT, true);
}

void AMyPlayerController::PressPowerAttack()
{
	character->SetAttackType(EAttackType::POWERATTACK);
}

void AMyPlayerController::UnPressPowerAttack()
{
	character->SetAttackType(EAttackType::BASICATTACK);
}

void AMyPlayerController::PressSkillAttack()
{
	character->SetAttackType(EAttackType::SKILLATTACK);
}

void AMyPlayerController::UnPressSkillAttack()
{
	character->SetAttackType(EAttackType::BASICATTACK);
}

void AMyPlayerController::PressLockOn()
{
	character->CallInputFunc(EPlayerState::NONE, EInputType::LOCKON, true);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::MOVE], ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::MOVE], ETriggerEvent::Started, this, &AMyPlayerController::PressMove);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::MOVE], ETriggerEvent::Completed, this, &AMyPlayerController::MoveEnd);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::ROTATE], ETriggerEvent::Triggered, this, &AMyPlayerController::Look);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SPRINT], ETriggerEvent::Triggered, this, &AMyPlayerController::PressSprint);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SPRINT], ETriggerEvent::Completed, this, &AMyPlayerController::UnPressSprint);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::DODGE], ETriggerEvent::Completed, this, &AMyPlayerController::Dodge);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::ATTACK], ETriggerEvent::Started, this, &AMyPlayerController::PressAttack);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::POWERATTACK], ETriggerEvent::Started, this, &AMyPlayerController::PressPowerAttack);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::POWERATTACK], ETriggerEvent::Completed, this, &AMyPlayerController::UnPressPowerAttack);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SKILL], ETriggerEvent::Started, this, &AMyPlayerController::PressSkillAttack);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SKILL], ETriggerEvent::Completed, this, &AMyPlayerController::UnPressSkillAttack);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SHIELD], ETriggerEvent::Started, this, &AMyPlayerController::PressShield);
	EnhancedInputComponent->BindAction(InputActionMap[EInputType::SHIELD], ETriggerEvent::Completed, this, &AMyPlayerController::UnPressShield);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::INTERACTION], ETriggerEvent::Started, this, &AMyPlayerController::PressInteraction);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::HEAL], ETriggerEvent::Started, this, &AMyPlayerController::PressHeal);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::QUIT], ETriggerEvent::Started, this, &AMyPlayerController::PressQuit);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::LOCKON], ETriggerEvent::Started, this, &AMyPlayerController::PressLockOn);
}
