#include "EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();

	MoveToTargetComp = CreateDefaultSubobject<UEnemyMoveToTargetComponent>(FName("Move to Target Comp"));

	MoveToLocationComp = CreateDefaultSubobject<UMoveToLocationComponent>(FName("Move to Location Comp"));

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>("Lock On Widget");
	LockOnWidget->SetupAttachment(GetMesh(), "Bip001-Spine2");

	StartAnimType = EMonsterAnimationType::NONE;

	for (EMonsterAnimationType AnimType : TEnumRange<EMonsterAnimationType>())
	{
		MontageEndEventMap.Add(AnimType, TFunction<void()>());
		NotifyEventMap.Add(AnimType, TMap<bool, TFunction<void()>>());
	}

	NotifyEventMap[EMonsterAnimationType::ATTACK_0].Add(true, [this]() {
		ActivateAttackCollision(ECollisionEnabled::QueryOnly);
		});
	NotifyEventMap[EMonsterAnimationType::ATTACK_0].Add(false, [this]() {
		ActivateAttackCollision(ECollisionEnabled::NoCollision);
		});

	NotifyEventMap[EMonsterAnimationType::POWERATTACK_0].Add(true, NotifyEventMap[EMonsterAnimationType::ATTACK_0][true]);
	NotifyEventMap[EMonsterAnimationType::POWERATTACK_0].Add(false, NotifyEventMap[EMonsterAnimationType::ATTACK_0][false]);


	// Add Empty Func
	for (EMonsterAnimationType AnimType : TEnumRange<EMonsterAnimationType>())
	{
		if (!NotifyEventMap[AnimType].Contains(false)) {
			NotifyEventMap[AnimType].Add(false, [this]() {});
		}

		if (!NotifyEventMap[AnimType].Contains(true)) {
			NotifyEventMap[AnimType].Add(true, [this]() {});
		}
	}
}


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	Controller = Cast<AAIController>(GetController());


	LockOnWidget->GetWidget()->SetVisibility(ESlateVisibility::Collapsed);

	if (StartAnimType == EMonsterAnimationType::STANDBY) {
	PlayMontageAnimation(StartAnimType);
	}

	for (int8 i = 0; i < PatrolActorArray.Num(); i++)
	{
		PatrolLocationArray.Add(PatrolActorArray[i]->GetActorLocation());
		PatrolActorArray[i]->Destroy();
	}

	PatrolActorArray.Empty();

	if (PatrolLocationArray.Num() > 0) {
		StartPatrol();
	}

	for (int8 i = 0; i < AttackCollisionArray.Num(); i++)
	{
		AttackCollisionArray[i]->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnWeaponOverlapBegin);
	}
	//EPathFollowingResult
	AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemyBase::MontageEnded);

	Controller->ReceiveMoveCompleted.AddDynamic(this, &AEnemyBase::ArriveTargetLocation);
	Controller->GetPathFollowingComponent()->OnRequestFinished.AddUFunction(this, FName("ArriveTargetLocation"));
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance->SetSpeed(FMath::Lerp(AnimInstance->GetSpeed(), GetVelocity().Length(), DeltaTime * 10.0f));
}

void AEnemyBase::EventNotify(bool IsBegin)
{
	NotifyEventMap[CurAnimType][IsBegin]();
}

void AEnemyBase::HitEvent()
{
	Super::HitEvent();
	MoveToTargetComp->Deactivate();
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	ActivateHitCollision(ECollisionEnabled::NoCollision);
	EnemyDataStruct.CharacterHp <= 0 ? PlayMontageAnimation(EMonsterAnimationType::DEAD) : PlayMontageAnimation(EMonsterAnimationType::HIT);
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	EnemyDataStruct.CharacterHp -= DamageAmount;
	Target = DamageCauser;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return EnemyDataStruct.CharacterHp;
}

void AEnemyBase::AttackTriggerBeginEvent(AActor* TargetActor)
{
	DetectPlayer(TargetActor);

	StartAttack();
}

void AEnemyBase::AttackTriggerEndEvent()
{
	ActiavteMovetoTargetComp();
}

void AEnemyBase::StartPatrol()
{
	MoveToLocationComp->SetLocation(PatrolLocationArray[CurPatrolIndex]);
	ActiavteMovetoLocationComp();
}

void AEnemyBase::StartAttack()
{
	if (AnimInstance->IsAnyMontagePlaying()) {
		return;
	}
	ActiavteMovetoTargetComp();
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	AnimInstance->Montage_Play(CurrentAttackMontage);
}

void AEnemyBase::DetectPlayer(AActor* TargetActor)
{
	Target = TargetActor;
	MoveToTargetComp->SetTarget(TargetActor);
	IsDetect = true;
}

void AEnemyBase::UnDetectPlayer()
{
	IsDetect = false;

	if (PatrolLocationArray.Num() > 0) {
		StartPatrol();
		return;
	}

	MoveToLocationComp->SetLocation(SpawnLocation);
	ActiavteMovetoLocationComp();
}

void AEnemyBase::ArriveTargetLocation(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult)
{
	if (MovementResult == EPathFollowingResult::Success && PatrolLocationArray.Num() > 0) {
		CurPatrolIndex < PatrolLocationArray.Num() - 1 ? CurPatrolIndex += 1 : CurPatrolIndex = 0;
		StartPatrol();
	}
}

void AEnemyBase::PlayMontageAnimation(EMonsterAnimationType type)
{
	SetAnimType(type);
	AnimInstance->Montage_Play(MontageMap[CurAnimType]);
}

void AEnemyBase::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted) {
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;

		if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > 150.0f) {
			ActiavteMovetoTargetComp();
			return;
		}

		Controller->StopMovement();
		MoveToTargetComp->Deactivate();

		if (NextAttackMontage) {
			StartAttack();
			NextAttackMontage = nullptr;
			return;
		}

		SetAnimType(EMonsterAnimationType::NONE);
	}
}

void AEnemyBase::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->TakeDamage(10.0f, CharacterDamageEvent, nullptr, this);
	ActivateAttackCollision(ECollisionEnabled::NoCollision);
}