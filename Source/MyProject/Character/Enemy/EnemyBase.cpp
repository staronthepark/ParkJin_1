#include "EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();

	MoveToTargetComp = CreateDefaultSubobject<UEnemyMoveToTargetComponent>(FName("Move to Target Comp"));
	MoveToTargetComp->SetComponentTickEnabled(false);

	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>("Lock On Widget");
	LockOnWidget->SetupAttachment(GetMesh(), "Bip001-Spine2");

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

	TArray<UActorComponent*> ActorCompArray = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName("Patrol"));
	for (int8 i = 0; i < ActorCompArray.Num(); i++)
	{
		PatrolPositionArray.Add(Cast<UPrimitiveComponent>(ActorCompArray[i]));
	}

	for (int8 i = 0; i < AttackCollisionArray.Num(); i++)
	{
		AttackCollisionArray[i]->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnWeaponOverlapBegin);
	}

	AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemyBase::MontageEnded);
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
	MoveToTargetComp->Activate();
}

void AEnemyBase::StartAttack()
{
	if (AnimInstance->IsAnyMontagePlaying()) {
		return;
	}
	MoveToTargetComp->Activate();
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

	if (PatrolPositionArray.Num() > 0) {
		Controller->MoveToLocation(PatrolPositionArray[CurPatrolIndex]->GetComponentLocation());
		return;
	}

	Controller->MoveToLocation(SpawnLocation);
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
			MoveToTargetComp->Activate();
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