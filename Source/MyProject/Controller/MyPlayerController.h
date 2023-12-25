// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Character/Player/MyProjectCharacter.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	AMyProjectCharacter* character;

	EPlayerState* PlayerState;

	UPROPERTY(EditAnyWhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TMap<EInputType, UInputAction*> InputActionMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay();

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void PressMove();

	void MoveEnd();
	void Dodge();

	void PressSprint();
	void UnPressSprint();

	void PressShield();
	void UnPressShield();

	void PressPowerAttack();
	void UnPressPowerAttack();

	void PressSkillAttack();
	void UnPressSkillAttack();

	void PressLockOn();

	void PressHeal();

	void PressAttack();

	void PressInteraction();

	void PressQuit();
};
