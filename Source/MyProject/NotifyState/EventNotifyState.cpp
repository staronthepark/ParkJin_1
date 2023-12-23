#include "EventNotifyState.h"

void UEventNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (Owner == nullptr) {
		Owner = Cast<ABaseCharacter>(MeshComp->GetOwner());
	}	

	if (Owner != nullptr) {
		Owner->EventNotify(true);
	}
}

void UEventNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (Owner != nullptr) {
		Owner->EventNotify(false);
	}
}