#include "../../CustomComponents/TickComponents/MoveToLocationComponent.h"

UMoveToLocationComponent::UMoveToLocationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
}


void UMoveToLocationComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
}


void UMoveToLocationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OwnerController->MoveToLocation(TargetLocation);
}