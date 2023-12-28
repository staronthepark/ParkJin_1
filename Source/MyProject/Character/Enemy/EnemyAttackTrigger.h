#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "EnemyAttackTrigger.generated.h"

UCLASS(meta = (DisplayName = "Enemy Attack Trigger", BlueprintSpawnableComponent))
class MYPROJECT_API UEnemyAttackTrigger : public UBoxComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	EMonsterAttackType AttackType;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	float CoolTime;

	bool IsCoolDown;

	AEnemyBase* Owner;

public:

	virtual void BeginPlay();

	void CoolDownEnd();

	void StartAttack();

	UFUNCTION()
	void TriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
