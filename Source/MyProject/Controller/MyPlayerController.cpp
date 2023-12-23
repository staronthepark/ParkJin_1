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
		character->StopPlayerMontage();
	}
}

void AMyPlayerController::MoveEnd()
{
	character->MoveEnd();
}

void AMyPlayerController::PressSprint()
{
	character->CallInputFunc(*PlayerState, EInputType::SPRINT, true);
}

void AMyPlayerController::UnPressSprint()
{
	character->CallInputFunc(*PlayerState, EInputType::SPRINT, false);
}

void AMyPlayerController::PressAttack()
{
	character->CallInputFunc(*PlayerState, EInputType::ATTACK, true);
}

void AMyPlayerController::PressLockOn()
{
	character->LockOn();
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

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::ATTACK], ETriggerEvent::Started, this, &AMyPlayerController::PressAttack);

	EnhancedInputComponent->BindAction(InputActionMap[EInputType::LOCKON], ETriggerEvent::Started, this, &AMyPlayerController::PressLockOn);
}
