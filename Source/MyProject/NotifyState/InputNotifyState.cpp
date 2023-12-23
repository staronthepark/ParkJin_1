// Fill out your copyright notice in the Description page of Project Settings.


#include "InputNotifyState.h"

void UInputNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (Owner == nullptr)
	{
		Owner = Cast<AMyProjectCharacter>(MeshComp);
	}

}

void UInputNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
