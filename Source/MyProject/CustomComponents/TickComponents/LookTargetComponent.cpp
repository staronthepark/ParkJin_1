#include "../TickComponents/LookTargetComponent.h"

ULookTargetComponent::ULookTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
}


void ULookTargetComponent::BeginPlay()
{
	Super::BeginPlay();

}


void ULookTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Direction = TargetComp->GetComponentLocation() - OwnerController->GetPawn()->GetActorLocation();
	FRotator Difference = FRotationMatrix::MakeFromX(Direction).Rotator();

	OwnerController->SetControlRotation(FMath::Lerp(OwnerController->GetControlRotation(), Difference, DeltaTime * 2.0f));
}