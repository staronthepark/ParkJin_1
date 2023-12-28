#pragma once

#include "CoreMinimal.h"
#include "../AnimInstance/BaseAnimInstance.h"
#include "DoorAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EDoorAnimationType : uint8
{
	NONE,
	OPEN,
	OPENLOOP,
	CLOSE,
	CLOSELOOP,
};

UCLASS()
class MYPROJECT_API UDoorAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

		UPROPERTY(Editanywhere, meta = (AllowPriaveAccess = true))
		EDoorAnimationType DoorAnimType;
	
public:
	FORCEINLINE void ChangeDoorAnimation(EDoorAnimationType type) {
		DoorAnimType = type;
	}
};
