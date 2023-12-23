#include "../NotifyState/CanclebyMoveNotifyState.h"

void UCanclebyMoveNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (Owner == nullptr)
	{
		Owner = Cast<AMyProjectCharacter>(MeshComp->GetOwner());
	}

	if (Owner != nullptr) {
		Owner->ActiveCanclebyMove(true);
		if (Owner->GetMovementVector() != FVector2D::ZeroVector) {
			Owner->StopPlayerMontage();
		}
	}
}

void UCanclebyMoveNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (Owner != nullptr) {
		Owner->ActiveCanclebyMove(false);
	}
}