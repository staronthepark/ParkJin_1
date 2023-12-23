// Fill out your copyright notice in the Description page of Project Settings.


#include "../Controller/MyPlayerController.h"
#include "EnhancedInputComponent.h"

void AMyPlayerController::BeginPlay()
{
	character = Cast<AMyProjectCharacter>(GetPawn());

}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	character->Move(Value);
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	character->Look(Value);
}


void AMyPlayerController::SetupInputComponent()
{

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(character->InputActionMap[EInputType::MOVE], ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
	EnhancedInputComponent->BindAction(character->InputActionMap[EInputType::ROTATE], ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
}
