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

	FORCEINLINE	void ActivateWalk(bool Value) { IsWalk = Value; }

	FORCEINLINE	void SetAxisX(float Value) { AxisX = Value; }
	FORCEINLINE	void SetAxisY(float Value) { AxisY = Value; }

	FORCEINLINE	float GetAxisX() { return AxisX; }
	FORCEINLINE	float GetAxisY() { return AxisY; }

	FORCEINLINE	void SetLockOn(bool Value) { IsLockOn = Value; }
};
