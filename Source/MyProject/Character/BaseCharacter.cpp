#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Init()
{
	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	IsDie = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Init();

	AttackCollisionArray.Empty();
	HitCollisionArray.Empty();

	TArray<UActorComponent*> ActorCompArray = GetComponentsByTag(UCapsuleComponent::StaticClass(), "Hit");

	for (int8 i = 0; i < ActorCompArray.Num(); i++)
	{
		HitCollisionArray.Add(Cast<UCapsuleComponent>(ActorCompArray[i]));
	}

	ActorCompArray = GetComponentsByTag(UBoxComponent::StaticClass(), "Weapon");

	for (int8 i = 0; i < ActorCompArray.Num(); i++)
	{
		AttackCollisionArray.Add(Cast<UBoxComponent>(ActorCompArray[i]));
	}

	ActivateAttackCollision(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::RespawnCharacter()
{
	if (IsDie)
	{
		return;
	}

	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	SetActive(true);
}

void ABaseCharacter::SetActive(bool value)
{
	SetActorHiddenInGame(!value);
	SetActorEnableCollision(value);
	SetActorTickEnabled(value);
}

void ABaseCharacter::ActivateAttackCollision(ECollisionEnabled::Type NewType)
{
	for (int8 i = 0; i < AttackCollisionArray.Num(); i++)
	{
		AttackCollisionArray[i]->SetCollisionEnabled(NewType);
	}
}

void ABaseCharacter::ActivateHitCollision(ECollisionEnabled::Type NewType)
{
	for (int8 i = 0; i < HitCollisionArray.Num(); i++)
	{
		HitCollisionArray[i]->SetCollisionEnabled(NewType);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitEvent();

	return DamageAmount;
}

void ABaseCharacter::HitEvent()
{
	HitStop();
}