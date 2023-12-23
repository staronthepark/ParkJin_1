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

	HitColliderArray = GetComponentsByTag(UCapsuleComponent::StaticClass(), "Hit");

	for (int32 i = 0; i < HitColliderArray.Num(); i++)
	{
		Cast<UCapsuleComponent>(HitColliderArray[i])->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::HitCollisionBeginOverlap);
	}
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

void ABaseCharacter::ActiveHitCollider(bool value)
{
	for (int32 i = 0; i < HitColliderArray.Num(); i++)
	{
		HitColliderArray[i]->Activate(value);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fDeltaTime = DeltaTime;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitStop();
	ActiveHitCollider(false);

	return DamageAmount;
}

void ABaseCharacter::HitEvent()
{
	ActiveHitCollider(false);

}

void ABaseCharacter::HitCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HitEvent();
}