#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class MYPROJECT_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsLockOn;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsWalk;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisX;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisY;


public:

	inline	void ActivateWalk(bool Value) { IsWalk = Value; }

	inline	void SetAxisX(float Value) { AxisX = Value; }
	inline	void SetAxisY(float Value) { AxisY = Value; }

	inline	float GetAxisX() { return AxisX; }
	inline	float GetAxisY() { return AxisY; }

	inline	void SetLockOn(bool Value) { IsLockOn = Value; }
};
