#include "EnemyMoveToTargetComponent.h"

UEnemyMoveToTargetComponent::UEnemyMoveToTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
}

void UEnemyMoveToTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AAIController>(Cast<APawn>(GetOwner())->GetController());
}


void UEnemyMoveToTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OwnerController->MoveToActor(Target);
	OwnerController->SetFocus(Target);
}