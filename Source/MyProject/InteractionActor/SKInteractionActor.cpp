#include "SKInteractionActor.h"

ASKInteractionActor::ASKInteractionActor()
{
	SKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SKMesh->SetupAttachment(MyRootComponent);
}

void ASKInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UDoorAnimInstance>(SKMesh->GetAnimInstance());
}

void ASKInteractionActor::ExecuteEvent()
{
	Super::ExecuteEvent();
	AnimInstance->ChangeDoorAnimation(EDoorAnimationType::OPEN);
}