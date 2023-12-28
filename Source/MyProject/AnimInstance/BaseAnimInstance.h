// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;

public:
	FORCEINLINE	void SetSpeed(float Value) { Speed = Value; }
	FORCEINLINE float GetSpeed() { return Speed; }
};
