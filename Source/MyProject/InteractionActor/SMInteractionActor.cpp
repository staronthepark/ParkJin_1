#include "SMInteractionActor.h"

ASMInteractionActor::ASMInteractionActor()
{
	SMMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SMMesh->SetupAttachment(RootComponent);
}

void ASMInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASMInteractionActor::ExecuteEvent()
{
	Super::ExecuteEvent();
}