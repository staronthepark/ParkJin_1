#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "MoveToLocationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMoveToLocationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMoveToLocationComponent();

	AAIController* OwnerController;

	FVector TargetLocation;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline void SetLocation(FVector Location) { TargetLocation = Location; }
};
