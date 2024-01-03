#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Character/Player/MyProjectCharacter.h"
#include "InputNotifyState.generated.h"

UCLASS()
class MYPROJECT_API UInputNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:

	UPROPERTY()
	AMyProjectCharacter* Owner;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
};