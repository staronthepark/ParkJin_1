#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Character/BaseCharacter.h"
#include "LookTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API ULookTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULookTargetComponent();

protected:
	virtual void BeginPlay() override;

	APlayerController* OwnerController;
	UPrimitiveComponent* TargetComp;

public:	

	void SetController(APlayerController* Controller) { OwnerController = Controller; }
	void SetTarget(UPrimitiveComponent* Target) { TargetComp = Target; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};