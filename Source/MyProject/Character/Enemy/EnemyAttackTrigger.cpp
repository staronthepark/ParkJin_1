#include "../Enemy/EnemyAttackTrigger.h"

void UEnemyAttackTrigger::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AEnemyBase>(GetOwner());

	OnComponentBeginOverlap.AddDynamic(this, &UEnemyAttackTrigger::TriggerOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UEnemyAttackTrigger::TriggerOverlapEnd);
}

void UEnemyAttackTrigger::CoolDownEnd()
{
	IsCoolDown = false;

	if (Owner->GetAttackType() == AttackType) {	
		StartAttack();
		Owner->PlayAnimMontage(AttackMontage);
	}
}

void UEnemyAttackTrigger::StartAttack()
{
	IsCoolDown = true;

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UEnemyAttackTrigger::CoolDownEnd, CoolTime);

	Owner->SetAttackType(AttackType);
}

void UEnemyAttackTrigger::TriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsCoolDown) {
		return;
	}

	Owner->AttackTriggerBeginEvent(AttackMontage, AttackType, OtherActor);

	StartAttack();
}

void UEnemyAttackTrigger::TriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Owner->AttackTriggerEndEvent();
}