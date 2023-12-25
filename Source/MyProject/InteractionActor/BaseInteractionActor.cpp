#include "BaseInteractionActor.h"

ABaseInteractionActor::ABaseInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MyRootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("My Root Component"));
	RootComponent = MyRootComponent;

//	TArray<>
	TArray<UActorComponent*> Array = GetComponentsByClass(UFXSystemComponent::StaticClass());

	for (int32 i = 0; i < Array.Num(); i++)
	{
		InteractionCompArray.Add(Cast<UFXSystemComponent>(Array[i]));
	}

}

void ABaseInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseInteractionActor::ExecuteEvent()
{
	for (int32 i = 0; i < InteractionCompArray.Num(); i++)
	{
		InteractionCompArray[i]->SetActive(true);
	}
}