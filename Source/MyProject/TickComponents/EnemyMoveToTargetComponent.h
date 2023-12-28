// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "EnemyMoveToTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UEnemyMoveToTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyMoveToTargetComponent();


private:
	AActor* Target;
	AAIController* OwnerController;

public:	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		

	FORCEINLINE void SetTarget(AActor* Actor) { Target = Actor; }
	FORCEINLINE void SetContoller(AAIController* Controller) { OwnerController = Controller; }
};
