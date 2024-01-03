#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), FName("Ref_Bip001-Head"));
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HeadBoneLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Head Bone Location"));
	HeadBoneLocation->SetupAttachment(GetMesh(), FName("Ref_Bip001-Head"));

	TargetDetectionCollison = CreateDefaultSubobject<USphereComponent>(TEXT("Enemy Detection Collider"));
	TargetDetectionCollison->SetupAttachment(RootComponent);
	TargetDetectionCollison->SetCollisionProfileName("Weapon");
	TargetDetectionCollison->SetSphereRadius(500.0f);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("Weapon_bone"));

	ShieldAttackOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Shield Attack Overlap"));
	ShieldAttackOverlap->SetupAttachment(GetMesh());

	ExecutionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Execution Trigger"));
	ExecutionTrigger->SetupAttachment(RootComponent);
	ExecutionTrigger->SetCollisionProfileName("Execution Trigger");

	//WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	//WeaponCollision->SetupAttachment(WeaponMesh);
	//WeaponCollision->SetCollisionProfileName("Weapon");

	ParryingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Collision"));
	ParryingCollision->SetupAttachment(GetMesh());
	ParryingCollision->SetCollisionProfileName("Parrying");

	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	ShieldMeshComp->SetupAttachment(GetMesh(), FName("POINT_SHIELD"));

	//WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box"));
	//WeaponOverlapStaticMeshCollision->SetupAttachment(WeaponMesh);
	//WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(WeaponMesh);
	SwordTrailComp->SetCollisionProfileName("Sword Trail");

	SkillTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Trail"));
	SkillTrailComp->SetupAttachment(WeaponMesh);
	SkillTrailComp->SetCollisionProfileName("Skill Trail");

	SkillAuraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Aura"));
	SkillAuraComp->SetupAttachment(WeaponMesh);
	SkillAuraComp->SetCollisionProfileName("Skill Aura");

	AttackAnimMap.Add(EAttackType::BASICATTACK, TMap<int8, EAnimationType>());
	AttackAnimMap[EAttackType::BASICATTACK].Add(0, EAnimationType::BASICATTACK_0);
	AttackAnimMap[EAttackType::BASICATTACK].Add(1, EAnimationType::BASICATTACK_1);
	AttackAnimMap[EAttackType::BASICATTACK].Add(2, EAnimationType::BASICATTACK_2);
	AttackAnimMap[EAttackType::BASICATTACK].Add(3, EAnimationType::BASICATTACK_3);

	AttackAnimMap.Add(EAttackType::POWERATTACK, TMap<int8, EAnimationType>());
	AttackAnimMap[EAttackType::POWERATTACK].Add(0, EAnimationType::POWERATTACK_0);
	AttackAnimMap[EAttackType::POWERATTACK].Add(1, EAnimationType::POWERATTACK_1);
	AttackAnimMap[EAttackType::POWERATTACK].Add(2, EAnimationType::POWERATTACK_2);

	AttackAnimMap.Add(EAttackType::SKILLATTACK, TMap<int8, EAnimationType>());
	AttackAnimMap[EAttackType::SKILLATTACK].Add(0, EAnimationType::SKILLATTACK_0);
	AttackAnimMap[EAttackType::SKILLATTACK].Add(1, EAnimationType::SKILLATTACK_1);

	PlayerActionTickMap.Add(EActionType::ATTACK, [this]()
		{
		});

	for (EPlayerState State : TEnumRange<EPlayerState>())
	{
		InputEventMap.Add(State, TMap<EInputType, TMap<bool, TFunction<void()>>>());
		for (EInputType MyInputType : TEnumRange<EInputType>())
		{
			InputEventMap[State].Add(MyInputType, TMap<bool, TFunction<void()>>());
		}
	}

	for (EAnimationType MyAnimType : TEnumRange<EAnimationType>())
	{
		NotifyEventMap.Add(MyAnimType, TMap<bool, TFunction<void()>>());
	}

//Input Event Map

	InputEventMap[EPlayerState::NONE][EInputType::SHIELD].Add(true, [this]() {

		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;

		SetStateType(EPlayerState::CANTACT);
		SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
		PlayMontageAnimation(EAnimationType::SHIELD);
		AnimInstance->ActivateWalk(true);
		});

	InputEventMap[EPlayerState::CANTACT][EInputType::SHIELD].Add(false, [this]() {
		if (AnimInstance->Montage_IsPlaying(MontageMap[EAnimationType::SHIELD])) {
		AnimInstance->Montage_Stop(0.2f, MontageMap[EAnimationType::SHIELD]);
		AnimInstance->ActivateWalk(false);
		SetSpeed(PlayerDataStruct.OriginSpeed);
		SetStateType(EPlayerState::NONE);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
		}
		});

	InputEventMap[EPlayerState::NONE][EInputType::LOCKON].Add(true, [this]() {
		if (CurAnimType != EAnimationType::SAVE) {
		LockOn();
		}
		});

	InputEventMap[EPlayerState::NONE][EInputType::QUIT].Add(true, [this]() {
		if (CurAnimType == EAnimationType::SAVE) {
			if (AnimInstance->Montage_GetCurrentSection() == FName("Loop")) {
			AnimInstance->Montage_JumpToSection(FName("End"), MontageMap[EAnimationType::SAVE]);
			}
		}
		});

	InputEventMap[EPlayerState::NONE][EInputType::INTERACTION].Add(true, [this]() {
		if (InteractionActor) {
			InteractionActor->ExecuteEvent();
			StartInteraction(InteractionActor->GetInteractionAnimType());
		}
		});

	InputEventMap[EPlayerState::NONE][EInputType::SPRINT].Add(true, [this]() {
		SprintBegin();
		});

	InputEventMap[EPlayerState::NONE][EInputType::HEAL].Add(true, [this]() {
		AnimInstance->ActivateWalk(true);
		ShieldMeshComp->SetVisibility(false);
		SetStateType(EPlayerState::CANTACT);
		PlayMontageAnimation(EAnimationType::HEAL);
		SetSpeed(PlayerDataStruct.PlayerWalkSpeed);
		});

	InputEventMap[EPlayerState::NONE][EInputType::SPRINT].Add(false, [this]() {
		SprintEnd();
		});

	InputEventMap[EPlayerState::NONE][EInputType::ATTACK].Add(true, [this]() {
		ComboAttack();
		});

	InputEventMap[EPlayerState::NONE][EInputType::DODGE].Add(true, [this]() {
		ComboAttackEnd();
		SetStateType(EPlayerState::CANTACT);
		MovementVector == FVector2D::ZeroVector ? PlayMontageAnimation(EAnimationType::BACKSTEP) : PlayMontageAnimation(EAnimationType::BASICDODGE);
		});

	InputEventMap[EPlayerState::AFTERATTACK][EInputType::DODGE].Add(true, [this]() {
 		InputEventMap[EPlayerState::NONE][EInputType::DODGE][true]();
		if (CurAnimType != EAnimationType::BACKSTEP) {
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
		}
		});

	InputEventMap[EPlayerState::AFTERATTACK][EInputType::ATTACK].Add(true, [this]() {
		if (CurAnimType == EAnimationType::BASICDODGE || CurAnimType == EAnimationType::BACKSTEP) {
			GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
			SetStateType(EPlayerState::CANTACT);
			PlayMontageAnimation(EAnimationType::DODGEATTACK);
			AttackEvent();
		}
		else {
			InputEventMap[EPlayerState::NONE][EInputType::ATTACK][true]();
		}
		});

	InputEventMap[EPlayerState::SPRINT][EInputType::ATTACK].Add(true, [this]() {
		SetStateType(EPlayerState::CANTACT);
		PlayMontageAnimation(EAnimationType::RUNATTACK);
		PlayerCurAttackIndex += 1;
		AttackEvent();
		});

	InputEventMap[EPlayerState::SPRINT][EInputType::SPRINT].Add(false, InputEventMap[EPlayerState::NONE][EInputType::SPRINT][false]);


//Notify Event Map

	NotifyEventMap[EAnimationType::BASICATTACK_0].Add(true, [this]() {
		CurStateType = EPlayerState::CANTACT;
		AttackEvent();
		});
	NotifyEventMap[EAnimationType::BASICATTACK_0].Add(false, [this]() {
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		ActivateAttackCollision(ECollisionEnabled::NoCollision);
		});

	NotifyEventMap[EAnimationType::BASICATTACK_1].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::BASICATTACK_1].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);
	NotifyEventMap[EAnimationType::BASICATTACK_2].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::BASICATTACK_2].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);
	NotifyEventMap[EAnimationType::BASICATTACK_3].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::BASICATTACK_3].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);

	NotifyEventMap[EAnimationType::POWERATTACK_0].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::POWERATTACK_0].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);
	NotifyEventMap[EAnimationType::POWERATTACK_1].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::POWERATTACK_1].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);
	NotifyEventMap[EAnimationType::POWERATTACK_2].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::POWERATTACK_2].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);

	NotifyEventMap[EAnimationType::SKILLATTACK_0].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::SKILLATTACK_0].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);
	NotifyEventMap[EAnimationType::SKILLATTACK_1].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::SKILLATTACK_1].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);

	NotifyEventMap[EAnimationType::DODGEATTACK].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::DODGEATTACK].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);

	NotifyEventMap[EAnimationType::RUNATTACK].Add(true, NotifyEventMap[EAnimationType::BASICATTACK_0][true]);
	NotifyEventMap[EAnimationType::RUNATTACK].Add(false, NotifyEventMap[EAnimationType::BASICATTACK_0][false]);


	NotifyEventMap[EAnimationType::BASICDODGE].Add(true, [this]() {
		Imotal = true;
		});
	NotifyEventMap[EAnimationType::BASICDODGE].Add(false, [this]() {
		Imotal = false;
		SetStateType(EPlayerState::AFTERATTACK);
		});

	NotifyEventMap[EAnimationType::HEAL].Add(true, [this]() {

		});
	NotifyEventMap[EAnimationType::HEAL].Add(false, [this]() {
		ShieldMeshComp->SetVisibility(true);
		AnimInstance->ActivateWalk(false);
		SetSpeed(PlayerDataStruct.OriginSpeed);
		});

	NotifyEventMap[EAnimationType::BACKSTEP].Add(true, NotifyEventMap[EAnimationType::BASICDODGE][true]);
	NotifyEventMap[EAnimationType::BACKSTEP].Add(false, NotifyEventMap[EAnimationType::BASICDODGE][false]);


//Add Empty Func

	for (EAnimationType AnimType : TEnumRange<EAnimationType>())
	{
		if (!MontageEndEventMap.Contains(AnimType)) {
			MontageEndEventMap.Add(AnimType, []() {});
		}
	}

	for (EActionType MyActionType : TEnumRange<EActionType>())
	{
		if (!PlayerActionTickMap.Contains(MyActionType)) {
			PlayerActionTickMap.Add(MyActionType, []() {});
		}
	}

	for (EPlayerState StateType : TEnumRange<EPlayerState>())
	{
		for (EInputType InputType : TEnumRange<EInputType>())
		{
			if (!InputEventMap[StateType].Contains(InputType)) {
				InputEventMap[StateType].Add(InputType, TMap<bool, TFunction<void()>>());				
			}
			if (!InputEventMap[StateType][InputType].Contains(true)) {
				InputEventMap[StateType][InputType].Add(true, []() {});
			}
			if (!InputEventMap[StateType][InputType].Contains(false)) {
				InputEventMap[StateType][InputType].Add(false, []() {});
			}
		}
	}

	for (EAnimationType MyAnimType : TEnumRange<EAnimationType>())
	{
		if (!NotifyEventMap[MyAnimType].Contains(true)) {
			NotifyEventMap[MyAnimType].Add(true, []() {});
		}
		if (!NotifyEventMap[MyAnimType].Contains(false)) {
			NotifyEventMap[MyAnimType].Add(false, []() {});
		}
	}
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaxAttackIndex = 4;
	CurAttackType = EAttackType::BASICATTACK;
	CurStateType = EPlayerState::NONE;

	CombatManager = NewObject<UCombatManager>();
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SetSpeed(PlayerDataStruct.OriginSpeed);

	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnMontageEnded.AddDynamic(this, &AMyProjectCharacter::MontageEnded);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::CapsuleOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyProjectCharacter::CapsuleOverlapEnd);

	for (int8 i = 0; i < AttackCollisionArray.Num(); i++)
	{
		AttackCollisionArray[i]->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectCharacter::OnWeaponOverlapBegin);
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMyProjectCharacter::MoveEnd()
{
	MovementVector = FVector2D(0.0f, 0.0f);
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

float AMyProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitEvent();
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return PlayerDataStruct.CharacterHp;
}

void AMyProjectCharacter::HitEvent()
{
	Super::HitEvent();

	CameraShake();

	if (PlayerDataStruct.CharacterHp <= 0) {
		PlayMontageAnimation(EAnimationType::DEAD);
		SetStateType(EPlayerState::CANTACT);
		SetActionType(EActionType::DEAD);
		return;
	}

	SetStateType(EPlayerState::CANTACT);
	PlayMontageAnimation(EAnimationType::HIT);
}

void AMyProjectCharacter::CallInputFunc(EPlayerState state, EInputType type, bool IsPress)
{
	InputEventMap[state][type][IsPress]();
}

void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance->SetSpeed(GetVelocity().Length());

	AnimInstance->SetAxisX(FMath::Lerp(AnimInstance->GetAxisX(), MovementVector.X, fDeltaTime * 10.0f));
	AnimInstance->SetAxisY(FMath::Lerp(AnimInstance->GetAxisY(), MovementVector.Y, fDeltaTime * 10.0f));
}

void AMyProjectCharacter::LookTarget()
{
}

void AMyProjectCharacter::LockOn()
{
	IsLockOn = !IsLockOn;

	GetCharacterMovement()->bOrientRotationToMovement = !IsLockOn;
	bUseControllerRotationYaw = IsLockOn;

	IsLockOn ? LockOnBegin() : LockOnEnd();
}

void AMyProjectCharacter::LockOnBegin()
{
	AnimInstance->SetLockOn(true);

	SetSpeed(PlayerDataStruct.PlayerLockOnMoveSpeed);
}

void AMyProjectCharacter::ComboAttack()
{
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	CurStateType = EPlayerState::BEFOREATTACK;

	if (PlayerCurAttackIndex >= MaxAttackIndex || PlayerCurAttackIndex == 0) {
		PlayerCurAttackIndex = 0;
	}

	if (MontageMap.Contains(CurAnimType)) {
		AnimInstance->Montage_Stop(0.2f, MontageMap[CurAnimType]);
	}

	PlayMontageAnimation(AttackAnimMap[CurAttackType][PlayerCurAttackIndex++]);
}

void AMyProjectCharacter::LockOnEnd()
{
	AnimInstance->SetLockOn(false);

	if (!IsSprint) {
	SetSpeed(PlayerDataStruct.OriginSpeed);
	}
}

void AMyProjectCharacter::SprintBegin()
{
	SetStateType(EPlayerState::SPRINT);
	SetSpeed(PlayerDataStruct.PlayerRunSpeed);
}

void AMyProjectCharacter::SprintEnd()
{
	SetStateType(EPlayerState::NONE);
	SetSpeed(PlayerDataStruct.OriginSpeed);
}

void AMyProjectCharacter::StartInteraction(EAnimationType type)
{
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	SetStateType(EPlayerState::CANTACT);
	PlayMontageAnimation(type);
}

void AMyProjectCharacter::PlayMontageAnimation(EAnimationType type)
{
	CurAnimType = type;
	AnimInstance->Montage_Play(MontageMap[CurAnimType]);
}

void AMyProjectCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted) {
		SprintEnd();
		ComboAttackEnd();
		SetStateType(EPlayerState::NONE);
		SetActionType(EActionType::NONE);
		SetAnimType(EAnimationType::NONE);
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}

void AMyProjectCharacter::EventNotify(bool IsBegin)
{
	NotifyEventMap[CurAnimType][IsBegin]();
}

void AMyProjectCharacter::CapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	InteractionActor = Cast<ABaseInteractionActor>(OtherActor);
}

void AMyProjectCharacter::CapsuleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractionActor == OtherActor) {
		InteractionActor = nullptr;
	}
}

void AMyProjectCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* DamagedCharacter = Cast<ABaseCharacter>(OtherActor);

	CameraShake();

	if (OtherActor->TakeDamage(10.0f, CharacterDamageEvent, nullptr, this) <= 0) {
		CombatManager->RemoveDamagedCharacter(DamagedCharacter);
		return;
	}

	CombatManager->AddDamagedCharacter(DamagedCharacter);
}