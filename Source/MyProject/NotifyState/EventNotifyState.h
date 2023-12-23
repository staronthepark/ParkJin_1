// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/BaseCharacter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EventNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEventNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:

	ABaseCharacter* Owner;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
