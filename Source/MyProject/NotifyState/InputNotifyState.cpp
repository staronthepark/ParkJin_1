#include "InputNotifyState.h"

void UInputNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (Owner == nullptr)
	{
		Owner = Cast<AMyProjectCharacter>(MeshComp->GetOwner());
	}

	if (Owner != nullptr) {
		Owner->SetStateType(EPlayerState::AFTERATTACK);
	}
}

void UInputNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (Owner != nullptr) {
	}
}
