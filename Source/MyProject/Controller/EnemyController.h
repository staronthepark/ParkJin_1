#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../Character/Enemy/EnemyBase.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EnemyController.generated.h"

UCLASS()
class MYPROJECT_API AEnemyController : public AAIController
{
	GENERATED_BODY()


	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UAISenseConfig_Sight* SightConfig;

	bool IsDetectPlayer;

	AEnemyBase* Owner;

public:
	AEnemyController();

	virtual void BeginPlay() override;
	void OnPossess(APawn* pawn) override;
	
	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void OnUpdated(TArray<AActor*> const& ActorArray);
};
