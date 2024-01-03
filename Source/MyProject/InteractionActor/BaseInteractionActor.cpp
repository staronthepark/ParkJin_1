#include "BaseInteractionActor.h"
#include "Particles/ParticleSystemComponent.h"

ABaseInteractionActor::ABaseInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MyRootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("My Root Component"));
	RootComponent = MyRootComponent;
}

void ABaseInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionCompArray = GetComponentsByClass(UFXSystemComponent::StaticClass());
}

void ABaseInteractionActor::ExecuteEvent()
{
	for (int8 i = 0; i < InteractionCompArray.Num(); i++)
	{
		InteractionCompArray[i]->SetActive(true);
	}
}