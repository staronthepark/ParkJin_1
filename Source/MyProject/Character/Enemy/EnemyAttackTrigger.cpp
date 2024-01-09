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

	if (IsOverlap) {
		CoolDownStart();
		Owner->SetAttackMontage(AttackMontage);
		Owner->SetAnimType(AnimationType);
		Owner->StartAttack();
	}
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UEnemyAttackTrigger::CoolDownStart()
{
	IsCoolDown = true;

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UEnemyAttackTrigger::CoolDownEnd, CoolTime);
}

void UEnemyAttackTrigger::TriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsOverlap = true;

	Owner->SetAnimType(AnimationType);
	Owner->SetAttackMontage(AttackMontage);

	if (IsCoolDown) {
		return;
	}

	Owner->StartAttack();
	Owner->AttackTriggerBeginEvent(OtherActor);

	CoolDownStart();
}

void UEnemyAttackTrigger::TriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsOverlap = false;

	Owner->AttackTriggerEndEvent();
}