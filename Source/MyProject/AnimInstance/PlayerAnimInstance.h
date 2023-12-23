// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsLockOn;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisX;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisY;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;

public:

	FORCEINLINE	void SetAxisX(float Value) { AxisX = Value; }
	FORCEINLINE	void SetAxisY(float Value) { AxisY = Value; }

	FORCEINLINE	float GetAxisX() { return AxisX; }
	FORCEINLINE	float GetAxisY() { return AxisY; }

	FORCEINLINE	void SetSpeed(float Value) { Speed = Value; }
	FORCEINLINE	void SetLockOn(bool Value) { IsLockOn = Value; }
};
