#include "EnemyController.h"
#include "../Character/Player/MyProjectCharacter.h"

AEnemyController::AEnemyController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception Component")));
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEnemyBase>(GetPawn());
}

void AEnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

}

void AEnemyController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (GetWorld()->GetFirstPlayerController()->GetPawn() == Actor)
	{
		if (Stimulus.WasSuccessfullySensed()) {
			Owner->DetectPlayer(Actor);
			Owner->ActiavteMovetoTargetComp();
		}
		else {
			Owner->UnDetectPlayer();
		}
	}
}

void AEnemyController::OnUpdated(TArray<AActor*> const& ActorArray)
{
}