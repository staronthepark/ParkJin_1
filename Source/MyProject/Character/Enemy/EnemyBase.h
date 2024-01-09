#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../BaseCharacter.h"
#include "../../CustomComponents/TickComponents/EnemyMoveToTargetComponent.h"
#include "../../CustomComponents/TickComponents/MoveToLocationComponent.h"
#include "../../AnimInstance/EnemyAnimInstance.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EMonsterAnimationType : uint8
{
	NONE,
	STANDBY,
	HIT,
	DEAD,
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

USTRUCT(BlueprintType)
struct FEnemyDataStruct : public FCharacterStat
{
	GENERATED_BODY()



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

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UMoveToLocationComponent* MoveToLocationComp;

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	FEnemyDataStruct EnemyDataStruct;

	UPROPERTY(Editanywhere)
	UWidgetComponent* LockOnWidget;

	UPROPERTY(Editanywhere)
	TArray<AActor*>PatrolActorArray;

	UPROPERTY(Editanywhere)
	EMonsterAnimationType StartAnimType;

	UPROPERTY()
	UAnimMontage* NextAttackMontage;

	UPROPERTY()
	UAnimMontage* CurrentAttackMontage;

	AAIController* Controller;

	AActor* Target;

	EMonsterAnimationType CurAnimType;


	bool Acting;
	bool IsDetect;

	int8 CurPatrolIndex;

	//TMap<int, TFunction<void()>> MonsterMoveMap;
	TArray<FVector> PatrolLocationArray;
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

	void StartPatrol();

	void StartAttack();

	void DetectPlayer(AActor * TargetActor);

	void UnDetectPlayer();

	void ActiavteMovetoTargetComp() { 
		MoveToLocationComp->Deactivate();
		MoveToTargetComp->Activate();
	}

	void ActiavteMovetoLocationComp() {
		MoveToTargetComp->Deactivate();
		MoveToLocationComp->Activate();
	}
	
	void SetAttackMontage(UAnimMontage* montage) { AnimInstance->IsAnyMontagePlaying() ? NextAttackMontage = montage : CurrentAttackMontage = montage; }

	virtual void PlayMontageAnimation(EMonsterAnimationType type);

	virtual void HitEvent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EventNotify(bool IsBegin)override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	inline bool IsActing() { return Acting; }
	inline EMonsterAnimationType GetAnimType() { return CurAnimType; }
	inline void SetAnimType(EMonsterAnimationType Type) { CurAnimType = Type; }
	inline UWidgetComponent* GetLockOnWidgetComp() { return LockOnWidget; }

	UFUNCTION()
	void ArriveTargetLocation(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};