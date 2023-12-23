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
	bool IsDie;
	bool IsLockOn;

	float fDeltaTime;

	FTimerHandle HitStopTimer;

	FVector SpawnLocation;
	FRotator SpawnRotation;

	UPROPERTY(Editanywhere)
	TArray<UActorComponent*> HitColliderArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* WeaponCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UBoxComponent* WeaponOverlapStaticMeshCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* HitCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* TargetDetectionCollison;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* SwordTrailComp;

public:

	virtual void Init();
	virtual void HitStop() {}
	virtual void RespawnCharacter();
	virtual void SetActive(bool value);
	virtual void ActiveHitCollider(bool value);

	virtual void ActivateRightWeapon() {  }
	virtual void DeactivateRightWeapon() {  }
	virtual void ActivateSMOverlap() {  }
	virtual void DeactivateSMOverlap() {  }
	virtual void ResumeMontage() { }
	
	virtual void EventNotify(bool IsBegin) {}
	virtual void CheckMontageEndNotify() {}

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void HitEvent();

	void DeactivateHitCollision() {}

	UFUNCTION()
	void HitCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
