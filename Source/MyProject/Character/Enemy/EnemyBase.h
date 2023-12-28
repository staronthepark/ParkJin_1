#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../BaseCharacter.h"
#include "../../TickComponents/EnemyMoveToTargetComponent.h"
#include "../../AnimInstance/EnemyAnimInstance.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EMonsterAnimationType : uint8
{
	NONE,
	STANDBY,
	IDLE,
	ATTACK_0,
	POWERATTACK_0,
	RANGEATTACK_0,
	DASHATTACK_0,	
};

ENUM_RANGE_BY_FIRST_AND_LAST(EMonsterAnimationType, EMonsterAnimationType::NONE, EMonsterAnimationType::DASHATTACK_0);

UENUM(BlueprintType)
enum class EMonsterStateType : uint8
{
	NONE,
	BEFOREATTACK,
	AFTERATTACK,
	CANTACT,
};

ENUM_RANGE_BY_FIRST_AND_LAST(EMonsterStateType, EMonsterStateType::NONE, EMonsterStateType::CANTACT);

UENUM(BlueprintType)
enum class EMonsterActionType : uint8
{
	NONE,
	DODGE,
	ATTACK,
	POWERATTACK,
	MOVE,
	RUN,
	SPRINT,
	HIT,
	DEAD,
};

ENUM_RANGE_BY_FIRST_AND_LAST(EMonsterActionType, EMonsterActionType::NONE, EMonsterActionType::DEAD)

UENUM(BlueprintType)
enum class EMonsterAttackType : uint8
{
	NONE,
	ATTACK_0,
	POWERATTACK_0,
	RANGEATTACK_0,
	DASHATTACK_0,
};

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	ZOMBIE,
	KNIGHT,
	ELITEKNIGHT,
	NUN,
	BOSS,
};

UCLASS()
class MYPROJECT_API AEnemyBase : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UEnemyAnimInstance* AnimInstance;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	TMap<EMonsterAnimationType, UAnimMontage*>MontageMap;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UAIPerceptionStimuliSourceComponent* Stimulus;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UEnemyMoveToTargetComponent* MoveToTargetComp;

	UPROPERTY()
	UAnimMontage* NextAttackMontage;

	UPROPERTY()
	UAnimMontage* CurrentAttackMontage;

	AAIController* Controller;

	AActor* Target;

	EMonsterAnimationType CurAnimType;
	EMonsterAnimationType StartAnimType;
	EMonsterAttackType CurAttackType;

	bool Acting;
	bool IsDetect;

	uint8 CurPatrolIndex;

	//TMap<int, TFunction<void()>> MonsterMoveMap;
	TArray<UPrimitiveComponent*> PatrolPositionArray;
	TMap<EMonsterAnimationType, TFunction<void()>>MontageEndEventMap;
	TMap<EMonsterAnimationType, TMap<bool, TFunction<void()>>> NotifyEventMap;
	TMap<EMonsterActionType, TFunction<void()>> MonsterTickEventMap;
	//TMap<bool, TFunction<void()>> CheckDIstanceMap;
	//TMap<EMonsterAnimationType, TFunction<void(float percent)>> SetActionByRandomMap;
	TMap<EMonsterAttackType, TFunction<void()>>AttackEventMap;

public:
	AEnemyBase();

	void AttackTriggerBeginEvent(AActor* TargetActor);
	void AttackTriggerEndEvent();

	void StartAttack();

	void DetectPlayer(AActor * TargetActor);
	void UnDetectPlayer();

	virtual void PlayMontageAnimation(EMonsterAnimationType type);

	virtual void BeginPlay() override;
	virtual void HitStop() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool IsActing() { return Acting; }
	FORCEINLINE EMonsterAttackType GetAttackType() { return CurAttackType; }
	FORCEINLINE void SetAttackType(EMonsterAttackType Type) { CurAttackType = Type; }
	FORCEINLINE void SetAttackMontage(UAnimMontage* montage) { AnimInstance->IsAnyMontagePlaying() ? NextAttackMontage = montage : CurrentAttackMontage = montage; }

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);
};