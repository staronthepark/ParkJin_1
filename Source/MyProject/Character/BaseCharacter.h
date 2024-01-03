// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/Class.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
//#include ".\ObjectPool\ObjectPool.h"
//#include "BaseTriggerActor.h"
//#include ".\Manager\JesusGameInstance.h"
//#include "../AnimInstance/BaseAnimInstance.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacter.generated.h"



USTRUCT(BlueprintType)
struct FCharacterStat
{
	GENERATED_BODY()
public:
	UPROPERTY(Editanywhere)
	float CharacterHp;
	UPROPERTY(Editanywhere)
	float CharacterMaxHp;
	UPROPERTY(Editanywhere)
	float OriginSpeed;
};

UCLASS()
class MYPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	
protected:

	UPROPERTY(EditAnywhere)
	class USphereComponent* TargetDetectionCollison;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* SwordTrailComp;

	UPROPERTY(EditAnywhere)
	TArray<UCapsuleComponent*> HitCollisionArray;

	UPROPERTY(EditAnywhere)
	TArray<UBoxComponent*> AttackCollisionArray;

	UPROPERTY(EditAnywhere)
	float HitStopTime;

	bool IsDie;
	bool IsLockOn;
	bool Imotal;

	float fDeltaTime;

	FTimerHandle HitStopTimer;
	FDamageEvent CharacterDamageEvent;

	FVector SpawnLocation;
	FRotator SpawnRotation;

public:

	void HitStop() {
		GetMesh()->bPauseAnims = true;
		GetWorldTimerManager().SetTimer(HitStopTimer, [this]() {
			GetMesh()->bPauseAnims = false;
			}, HitStopTime, false);
	}

	virtual void Init();
	virtual void RespawnCharacter();
	virtual void SetActive(bool value);

	virtual void ActivateAttackCollision(ECollisionEnabled::Type NewType);
	virtual void ActivateHitCollision(ECollisionEnabled::Type NewType);

	virtual void ActivateSMOverlap() {  }
	virtual void DeactivateSMOverlap() {  }
	virtual void ResumeMontage() { }
	
	virtual void EventNotify(bool IsBegin) {}
	virtual void CheckMontageEndNotify() {}

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void HitEvent();
};
