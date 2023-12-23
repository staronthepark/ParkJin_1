#include "../NotifyState/CanclebyMoveNotifyState.h"

void UCanclebyMoveNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (Owner == nullptr)
	{
		Owner = Cast<AMyProjectCharacter>(MeshComp);
	}
}

void UCanclebyMoveNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}