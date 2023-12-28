#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
#include "../../AnimInstance/PlayerAnimInstance.h"
//#include "../UI/PlayerHUD.h"
//#include "../UI/UserSettingUI.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "InputActionValue.h"
#include "Containers/Queue.h"
#include "../../InteractionActor/BaseInteractionActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../Flag/Enums.h"
//#include "ActorSequenceComponent.h"
//#include "ActorSequencePlayer.h"
#include "MyProjectCharacter.generated.h"


USTRUCT(BlueprintType)
struct FPlayerDamageInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float Damage;

	UPROPERTY(EditAnywhere)
		float VibrateIntensity;

	UPROPERTY(EditAnywhere)
		float VibrateDuration;
};

USTRUCT(BlueprintType)
struct FCameraSetting
{
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere)
		float CameraBoomLength;

	UPROPERTY(EditAnyWhere)
		FVector CameraSocketOffset;
};

USTRUCT(BlueprintType)
struct FPlayerCharacterDataStruct : public FCharacterStat
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TMap<EAnimationType, FPlayerDamageInfo>DamageList;

	UPROPERTY(EditAnywhere)
		int32 MaxHealCount;

	UPROPERTY(EditAnywhere)
		float SoulCount;

	UPROPERTY(EditAnywhere)
		float MaxSoulCount;

	UPROPERTY(EditAnywhere)
		float MaxStamina;

	UPROPERTY(EditAnywhere)
		float PlayerStamina;

	UPROPERTY(EditAnywhere)
		float PlayerRunSpeed;

	UPROPERTY(EditAnywhere)
		float PlayerWalkSpeed;

	UPROPERTY(EditAnywhere)
		float PlayerLockOnMoveSpeed;

	UPROPERTY(EditAnywhere)
		float StaminaRecovery;

	UPROPERTY(EditAnywhere)
		float PlayerHealValue;

	UPROPERTY(EditAnywhere)
		float PlayerRunStamina;

	UPROPERTY(EditAnywhere)
		float PlayerExecutionFirstDamage;

	UPROPERTY(EditAnywhere)
		float PlayerExecutionSecondDamage;

	UPROPERTY(EditAnywhere)
		float DeployShieldStaminaReduce;

	UPROPERTY(EditAnywhere)
		float ShieldDashMoveDistance;

	UPROPERTY(EditAnywhere)
		float BaseDamage;

	UPROPERTY(EditAnywhere)
		float ShieldCoolDown;

	UPROPERTY(EditAnywhere)
		float SkillCoolDown;

	UPROPERTY(EditAnywhere)
		float ShieldDecreaseSoulPercent;

	UPROPERTY(EditAnywhere)
		float PowerAttackDefDistance;

	UPROPERTY(EditAnywhere)
		float AttackDefDistance;

	UPROPERTY(EditAnywhere)
		int32 StrengthIndex;

	UPROPERTY(EditAnywhere)
		int32 StaminaIndex;

	UPROPERTY(EditAnywhere)
		int32 HPIndex;

	UPROPERTY(EditAnywhere)
		int32 ShieldIndex;

	UPROPERTY(EditAnywhere)
		int32 SoulBonusCount;

	UPROPERTY(EditAnywhere)
		int32 SkillSoulCost;

	UPROPERTY(EditAnywhere)
		int32 ShieldBashSoulCost;
};

class UInputAction;

UCLASS(config=Game)
class AMyProjectCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UNiagaraComponent* SkillTrailComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UNiagaraComponent* SkillAuraComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UBoxComponent* SkillCollisionComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UBoxComponent* ExecutionTrigger;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UBoxComponent* ShieldAttackOverlap;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* PlayerSKMesh;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* HeadBoneLocation;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	TSubclassOf< UCameraShakeBase> PlayerDoorCameraShake;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	FPlayerCharacterDataStruct PlayerDataStruct;

	UPROPERTY(EditAnyWhere, Meta = (AllowPrivateAccess = true))
	TMap<EAnimationType, UAnimMontage*> MontageMap;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UBoxComponent* ParryingCollision;

	UPROPERTY(EditAnyWhere, Meta = (AllowPrivateAccess = true))
	TMap<EActionType, float>PlayerUseStaminaMap;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY()
	UBoxComponent* ShieldOverlapComp;

	//	UPROPERTY()
	//	TSubclassOf<UPlayerHUD> PlayerUIClass;

		//UPROPERTY()
		//UPlayerHUD* PlayerHUD;

	//	UPROPERTY()
	//	UActorSequencePlayer* BossExecutionSequncePlayer;

	UPROPERTY()
	UPlayerAnimInstance* AnimInstance;

	FCameraSetting OriginCameraSetting;
	FCameraSetting ShieldCameraSetting;
	FCameraSetting CurrentCameraSetting;

	int32 CurRotateIndex;
	int32 CurHealCount;
	int32 PlayerCurAttackIndex;
	int32 MaxAttackIndex;
	int32 AxisX;
	int32 AxisY;

	float RotSpeed;
	float DiagonalSpeed;
	float ShieldDashSpeed;
	float CameraDistanceToPlayer;
	float ChangeTargetTime;
	float TargetOpacity;
	float ForwardMovementValue;
	float RightMovementValue;
	float HeadYawRoation;

	bool IsCollisionCamera;
	bool CanExecution;
	bool IsExecute;
	bool CanNextAttack;
	bool IsSprint;
	bool IsDead;
	bool IsGrab;
	bool CanShieldDeploy;
	bool CanUseSkill;

	bool CanclebyMove;

	FVector PlayerForwardDirection;
	FVector PlayerRightDirection;
	FVector ExecuteDirection;
	FVector ExecuteLocation;
	FVector TargetDirection;
	FVector OriginLocation;
	FVector CamExecutionDirection;
	FVector2D MovementVector;

	FRotator YawRotation;
	FRotator Difference;
	FRotator OriginRotation;

	FTimerHandle ShieldCoolDownTimer;
	FTimerHandle SkillCoolDownTimer;
	FTimerHandle SprintStartTimer;
	FTimerHandle SprintEndTimer;
	FTimerHandle DeadTimer;

	ABaseCharacter* ExecutionCharacter;
	ABaseInteractionActor* InteractionActor;

	EPlayerState CurStateType;
	EActionType CurActionType;
	EAttackType CurAttackType;
	EAnimationType CurAnimType;

	TArray<TArray<float>> ForwardRotation;
	TArray<TArray<EAnimationType>> MovementAnim;
	TArray<TArray<EAnimationType>> DodgeDirection;

	TArray<UPrimitiveComponent*>TargetCompArray;
	TArray<UPrimitiveComponent*>TargetCompInScreenArray;
	TArray<UPrimitiveComponent*>TargetCompFrontPlayerArray;

	//TMap<EAnimationType, EPlayerState>PlayerEnumToAnimTypeMap;
	TMap<EAnimationType, TMap<bool, TFunction<void()>>> NotifyEventMap;
	TMap<EActionType, TFunction<void()>> PlayerActionTickMap;
	TMap<EAnimationType, TFunction<void()>>MontageEndEventMap;

	TMap<EActionType, TMap< int32, EAnimationType>>IntToEnumMap;

	TMap<EAnimationType, FRotator> HitEffectRotatorList;

	TMap<bool, TFunction<EAnimationType()>> DodgeAnimationMap;
	TMap<bool, TFunction<void()>> MovementAnimMap;

	TMap<EAnimationType, TMap<bool, TFunction<void()>>> PlayerEventFuncMap;
	TMap<bool, TFunction<void()>> PlayerAttackFuncMap;
	TMap<EPlayerState, TMap<EInputType, TMap<bool, TFunction<void()>>>>InputEventMap;
	//	TMap<EPlayerStatType, int32> StatCurrentIdxMap;
	TMap<EActionType, int32>PlayerMaxAttackIndex;
	TMap<bool, TMap<bool, float>>SpeedMap;
	TMap<bool, TFunction<void()>> LockOnCameraSettingMap;
	TMap<EAttackType, TMap<int32, EAnimationType>> AttackAnimMap;

public:
	AMyProjectCharacter();

	virtual void BeginPlay();

	virtual void EventNotify(bool IsBegin);

	//virtual bool IsAlive() {}override;
	//
	//
	//virtual void CheckMontageEndNotify() {}override;
	//
	//virtual void ResumeMontage() {}override;
	//
	//virtual void HitStop() {}override;
	//
	//
	virtual void Tick(float DeltaTime);
	//
	//virtual void RespawnCharacter() {}override;
	//
	//virtual void EventNotify(bool value) {};
	//
	//virtual void PlayExecutionAnimation() {}override;
	//
	//virtual void ActivateRightWeapon() {}override;
	//
	//virtual void DeactivateRightWeapon() {}override;
	//
	//virtual void ActivateSMOverlap() {}override;
	//
	//virtual void DeactivateSMOverlap() {}override;
	//
	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {} override;


	void CallInputFunc(EPlayerState state, EInputType type, bool IsPress);

	void Move(const FInputActionValue& Value);
	void MoveEnd();

	void Look(const FInputActionValue& Value);

	void GetCompsInScreen(TArray<UPrimitiveComponent*>Array) {}

	void GetFirstTarget() {}

	void ChangeTarget(ECameraDirection Direction) {}

	void RayCastOnTargets() {}

	void ShoulderView(bool Value) {}

	void SetInputType(bool IsPad) {}

	void UseItem() {}

	void LookTarget();

	void ResetGame() {}

	void SetPlayerForwardRotAndDir() {}

	void SetPlayerRightRotAndDir() {}

	void RotatePlayer() {}

	void PlayerMovement() {}

	void Parring() {}

	void LoadMonster(FString name) {}

	void FadeIn() {}	

	void ComboAttack();

	void Dodge() {}

	void RestoreStat() {}

	void ActivateCollision() {}

	void MoveSpawnLocation(FVector Location) {}

	void LockOnBegin();
	void LockOn();
	void LockOnEnd();

	void SprintBegin();
	void SprintEnd();

	void StartInteraction(EAnimationType type);

	void PlayMontageAnimation(EAnimationType type);

	void Run() {}

	void SetShieldHP(float HP, FVector Location) {}

	void RecoverStamina() {}

	void RecoverShield() {}

	void RecoverSkill() {}

	void CheckInputKey() {}

	void ShieldOff() {}

	void ShieldOn() {}

	void ChangeActionType(EActionType type) {}

	void SetSprint() {}

	void PlayerShieldDashMovement() {}

	void FadeOut() {}

	void SetCameraTarget(FVector Offset, float Length) {}

	void ShieldAttack() {}

	void SetSoul(int32 value) {}

	void LoadFile() {}

	void LoadMap() {}

	void AfterAttackNotify2(bool value) { }

	void PlayerDead(bool IsFly) {  }

	float GetPercent(float value, float min, float max) { return 0; }

	bool UseStamina(float value) { return 0; }

	bool CanActivate(int32 SoulCount) { return 0; }
	
	FORCEINLINE FVector2D GetMovementVector() { return MovementVector; }

	FORCEINLINE EPlayerState * GetCurStateType() { return &CurStateType; }

	FORCEINLINE bool IsActiveCanclebyMove() { return CanclebyMove; }

	FORCEINLINE void SetStateType(EPlayerState type) { CurStateType = type; }

	FORCEINLINE UPlayerAnimInstance* GetAnimInstance() { return AnimInstance; }

	FORCEINLINE void ActiveCanclebyMove(bool value) { CanclebyMove = value; }

	FORCEINLINE void SetAnimType(EAnimationType type) { CurAnimType = type; }

	FORCEINLINE void SetActionType(EActionType type) { CurActionType = type; }

	FORCEINLINE void SetSpeed(float speed) { GetCharacterMovement()->MaxWalkSpeed = speed; }

	FORCEINLINE void ComboAttackEnd() { 
		PlayerCurAttackIndex = 0; 
		CanclebyMove = false;
	}

	FORCEINLINE void StopPlayerMontage() {
		AnimInstance->Montage_Stop(0.2f, MontageMap[CurAnimType]);
		MontageEnded(nullptr, false);
	}

	FORCEINLINE void SetAttackType(EAttackType type) {
		CurAttackType = type;
		if (CurAttackType == EAttackType::POWERATTACK) {
			if (PlayerCurAttackIndex == 3) {
				PlayerCurAttackIndex = 1;
			}
			MaxAttackIndex = 3;
		}
		else if (CurAttackType == EAttackType::SKILLATTACK) {
			MaxAttackIndex = 2;
		}
		else {
			MaxAttackIndex = 4;
		}
	}

	UFUNCTION()
		void OnEnemyDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		void OnEnemyDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UFUNCTION()
		void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		void OnSMOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UFUNCTION()
		void OnSMOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		void OnExecutionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		void OnExecutionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	UFUNCTION()
		void OnParryingOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
		void OnShieldOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
			
	UFUNCTION()
		void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void CapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CapsuleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};