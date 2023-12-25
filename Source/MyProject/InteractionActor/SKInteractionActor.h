#pragma once

#include "CoreMinimal.h"
#include "BaseInteractionActor.h"
#include "../AnimInstance/DoorAnimInstance.h"
#include "SKInteractionActor.generated.h"

UCLASS()
class MYPROJECT_API ASKInteractionActor : public ABaseInteractionActor
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* SKMesh;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
		UDoorAnimInstance* AnimInstance;

public:

	ASKInteractionActor();

	virtual void BeginPlay() override;
	virtual void ExecuteEvent() override;
};