#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Character/Player/MyProjectCharacter.h"
#include "CanclebyMoveNotifyState.generated.h"

UCLASS()
class MYPROJECT_API UCanclebyMoveNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
private:

	AMyProjectCharacter* Owner;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
