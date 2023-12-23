// Copyright Epic Games, Inc. All Rights Reserved.

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

	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh"));
	ShieldMeshComp->SetupAttachment(GetMesh(), FName("POINT_SHIELD"));

	ShieldAttackOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Shield Attack Overlap"));
	ShieldAttackOverlap->SetupAttachment(GetMesh());

	ExecutionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Execution Trigger"));
	ExecutionTrigger->SetupAttachment(RootComponent);
	ExecutionTrigger->SetCollisionProfileName("Execution Trigger");

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName("Weapon");

	SkillCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collision"));
	SkillCollisionComp->SetupAttachment(WeaponMesh);
	SkillCollisionComp->SetCollisionProfileName("Skill Collision");

	ParryingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Parrying Collision"));
	ParryingCollision->SetupAttachment(GetMesh());
	ParryingCollision->SetCollisionProfileName("Parrying");

	//WeaponOverlapStaticMeshCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Box"));
	//WeaponOverlapStaticMeshCollision->SetupAttachment(WeaponMesh);
	//WeaponOverlapStaticMeshCollision->SetCollisionProfileName("Weapon");

	SwordTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Sword Trail"));
	SwordTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SwordTrailComp->SetCollisionProfileName("Sword Trail");

	SkillTrailComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Trail"));
	SkillTrailComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SkillTrailComp->SetCollisionProfileName("Skill Trail");

	SkillAuraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SKill Aura"));
	SkillAuraComp->SetupAttachment(GetMesh(), FName("Weapon_bone"));
	SkillAuraComp->SetCollisionProfileName("Skill Aura");

	ShieldEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Shield Effect Comp"));
	ShieldEffectComp->SetupAttachment(GetMesh(), FName("POINT_SHIELD"));
	ShieldEffectComp->SetCollisionProfileName("Shield Effect Comp");

	PlayerActionTickMap.Add(EActionType::ATTACK, [&]()
		{
			//RecoverStamina();
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

	NotifyEventMap[EAnimationType::BASICATTACK].Add(true, [&]()
		{
			CurStateType = EPlayerState::CANTACT;
		});
	NotifyEventMap[EAnimationType::POWERATTACK].Add(true, NotifyEventMap[EAnimationType::BASICATTACK][true]);

//	InputEventMap[EPlayerState::NONE][EActionType::SKILL].Add(true, [&]()
//		{
//			//if (IsGrab)
//			//	Parring();
//			//else
//			//	SkillAttack();
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::DODGE].Add(true, [&]()
//		{
//			//Dodge();
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::ATTACK].Add(true, [&]()
//		{
//			//BasicAttack();
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::POWERATTACK].Add(true, [&]()
//		{
//			//PowerAttack();
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::PARRING].Add(true, [&]()
//		{
//			//if (IsGrab)
//			//	Parring();
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::MOVE].Add(true, [&]()
//		{
//			//ChangActionType(EActionType::MOVE);
//			//SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
//			//AnimInstance->PlayerAnimationType = AnimationType::NONE;
//		});
//	InputEventMap[EPlayerState::NONE][EActionType::MOVE].Add(false, [&]()
//		{
//			//if (AxisX == 1 && AxisY == 1)
//			//{
//			//	SetSpeed(0);
//			//	ChangeMontageAnimation(AnimationType::ENDOFRUN);
//			//	ChangActionType(EActionType::NONE);
//			//}
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::HEAL].Add(true, [&]()
//		{
//			//if (CurHealCount > 0 && !IsHeal)
//			//{
//			//	UseItem();
//			//	IsHeal = true;
//			//	GetCharacterMovement()->MaxWalkSpeed = PlayerDataStruct.PlayerWalkSpeed;
//			//}
//		});
//
//	InputEventMap[EPlayerState::NONE][EActionType::SHIELD].Add(true, [&]()
//		{
//			//if (!CanShieldDeploy)return;
//			//if (PlayerDataStruct.SoulCount <= 0 || !CanShieldDeploy)return;
//			//ChangeMontageAnimation(AnimationType::SHIELDSTART);
//			//
//			//if (AxisY != 1 || AxisX != 1)
//			//{
//			//	AnimInstance->BodyBlendAlpha = 0.0f;
//			//	ChangActionType(EActionType::MOVE);
//			//}
//			//else
//			//{
//			//	ChangActionType(EActionType::NONE);
//			//}
//			//
//			//IsCollisionCamera = false;
//			//ComboAttackEnd();
//			//GetCharacterMovement()->MaxWalkSpeed = PlayerDataStruct.PlayerWalkSpeed;
//			//
//			//IsGrab = true;
//			//ShieldOn();
//			//ShieldOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//			//CameraBoom11->CameraLagSpeed = 30.0f;
//			//SetCameraTarget(GrabSocketOffset, GrabCameraLength);
//		});
//	InputEventMap[EPlayerState::NONE][EActionType::SHIELD].Add(false, [&]()
//		{
//			//if (!IsGrab)return;
//			//IsGrab = false;
//			//if (AxisY == 1 && AxisX == 1)
//			//{
//			//	ChangeMontageAnimation(AnimationType::SHIELDEND);
//			//}
//			//else
//			//{
//			//	SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
//			//	PlayerCurAction = EPlayerState::RUN;
//			//}
//			//
//			//AnimInstance->BodyBlendAlpha = 1.0f;
//			//ShieldOff();
//			//ShoulderView(IsShoulderView);
//		});
//	InputEventMap[EPlayerState::NONE][EActionType::INTERACTION].Add(false, [&]()
//		{
//			//if (!IsGrab)return;
//			//IsGrab = false;
//			//if (AxisY == 1 && AxisX == 1)
//			//{
//			//	ChangeMontageAnimation(AnimationType::SHIELDEND);
//			//}
//			//else
//			//{
//			//	SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
//			//	PlayerCurAction = EPlayerState::RUN;
//			//}
//			//
//			//AnimInstance->BodyBlendAlpha = 1.0f;
//			//ShieldOff();
//			//ShoulderView(IsShoulderView);
//		});
//	InputEventMap[EPlayerState::NONE][EActionType::INTERACTION].Add(false, [&]()
//		{
//			//if (!IsGrab)return;
//			//IsGrab = false;
//			//if (AxisY == 1 && AxisX == 1)
//			//{
//			//	ChangeMontageAnimation(AnimationType::SHIELDEND);
//			//}
//			//else
//			//{
//			//	SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
//			//	PlayerCurAction = EPlayerState::RUN;
//			//}
//			//
//			//AnimInstance->BodyBlendAlpha = 1.0f;
//			//ShieldOff();
//			//ShoulderView(IsShoulderView);
//		});
//
//	InputEventMap[EPlayerState::BEFOREATTACK][EActionType::MOVE].Add(true, InputEventMap[EPlayerState::NONE][EActionType::MOVE][true]);
//	InputEventMap[EPlayerState::BEFOREATTACK][EActionType::MOVE].Add(false, InputEventMap[EPlayerState::NONE][EActionType::MOVE][false]);
//
//	InputEventMap[EPlayerState::RUN][EActionType::SKILL].Add(true, InputEventMap[EPlayerState::NONE][EActionType::SKILL][true]);
//	InputEventMap[EPlayerState::RUN][EActionType::ATTACK].Add(true, InputEventMap[EPlayerState::NONE][EActionType::ATTACK][true]);
//	InputEventMap[EPlayerState::RUN][EActionType::POWERATTACK].Add(true, InputEventMap[EPlayerState::NONE][EActionType::POWERATTACK][true]);
//	InputEventMap[EPlayerState::RUN][EActionType::PARRING].Add(true, InputEventMap[EPlayerState::NONE][EActionType::PARRING][true]);
//	InputEventMap[EPlayerState::RUN][EActionType::MOVE].Add(true, InputEventMap[EPlayerState::NONE][EActionType::MOVE][true]);
//	InputEventMap[EPlayerState::RUN][EActionType::MOVE].Add(false, InputEventMap[EPlayerState::NONE][EActionType::MOVE][false]);
//
//	InputEventMap[EPlayerState::RUN][EActionType::HEAL].Add(true, InputEventMap[EPlayerState::NONE][EActionType::HEAL][true]);
//	//InputEventMap[EPlayerState::RUN][EActionType::INTERACTION].Add(true, InputEventMap[EPlayerState::NONE][EActionType::INTERACTION][true]);
//	//InputEventMap[EPlayerState::RUN][EActionType::INTERACTION].Add(false, InputEventMap[EPlayerState::NONE][EActionType::INTERACTION][false]);
//	InputEventMap[EPlayerState::RUN][EActionType::SHIELD].Add(true, [&]()
//		{
//			//InputEventMap[EPlayerState::NONE][EActionType::SHIELD][true]();
//			//if (IsGrab)
//			//	AnimInstance->BodyBlendAlpha = 0.0f;
//
//		});
//	InputEventMap[EPlayerState::RUN][EActionType::SHIELD].Add(false, [&]()
//		{
//			//InputEventMap[EPlayerState::NONE][EActionType::SHIELD][false]();
//		});
//
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::DODGE].Add(true, InputEventMap[EPlayerState::NONE][EActionType::DODGE][true]);
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::ATTACK].Add(true, InputEventMap[EPlayerState::NONE][EActionType::ATTACK][true]);
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::POWERATTACK].Add(true, InputEventMap[EPlayerState::NONE][EActionType::POWERATTACK][true]);
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::PARRING].Add(true, InputEventMap[EPlayerState::NONE][EActionType::PARRING][true]);
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::MOVE].Add(true, [&]()
//		{
//			//if (!CancleByMove || AnimInstance->PlayerAnimationType == AnimationType::SUPERHIT)return;
//			//SetSpeed(SpeedMap[IsLockOn || IsGrab][false]);
//			//ChangActionType(EActionType::MOVE);
//			//ComboAttackEnd();
//			//CancleByMove = false;
//		});
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::MOVE].Add(false, [&]()
//		{
//			//if (!CancleByMove || AnimInstance->PlayerAnimationType == AnimationType::SUPERHIT)return;
//			//if (CurEActionType == EActionType::MOVE)
//			//{
//			//	if (AxisX == 1 && AxisY == 1)
//			//	{
//			//		SetSpeed(0);
//			//		ChangActionType(EActionType::NONE);
//			//	}
//			//	CancleByMove = false;
//			//}
//		});
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::SHIELD].Add(true, InputEventMap[EPlayerState::NONE][EActionType::SHIELD][true]);
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::SHIELD].Add(false, InputEventMap[EPlayerState::NONE][EActionType::SHIELD][false]);
//
//	InputEventMap[EPlayerState::AFTERATTACK][EActionType::SKILL].Add(true, InputEventMap[EPlayerState::NONE][EActionType::SKILL][true]);
//
//	InputEventMap[EPlayerState::CANWALK][EActionType::MOVE].Add(true, [&]()
//		{
//			//ChangActionType(EActionType::MOVE);
//			//GetCharacterMovement()->MaxWalkSpeed = PlayerDataStruct.PlayerWalkSpeed;
//			//AnimInstance->BodyBlendAlpha = 0.0f;
//		});
//	InputEventMap[EPlayerState::CANWALK][EActionType::MOVE].Add(false, [&]()
//		{
//			//if (AxisY == 1 && AxisX == 1)
//			//	AnimInstance->BodyBlendAlpha = 1.0f;
//
//		});
//	InputEventMap[EPlayerState::CANWALK][EActionType::SHIELD].Add(false, InputEventMap[EPlayerState::NONE][EActionType::SHIELD][false]);
//
//	InputEventMap[EPlayerState::CANTACT][EActionType::SHIELD].Add(false, [&]()
//		{
//			//	IsGrab = false;
//		});
//
//
//	InputEventMap[EPlayerState::CANATTACK][EActionType::ATTACK].Add(true, [&]()
//		{
//			//BasicAttack();
//		});
//
//	InputEventMap[EPlayerState::SPRINT][EActionType::SKILL].Add(true, [&]()
//		{
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::SKILL].Add(false, [&]()
//		{
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::INTERACTION].Add(true, [&]()
//		{
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::INTERACTION].Add(false, [&]()
//		{
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::DODGE].Add(true, [&]()
//		{
//			//Dodge();
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::DODGE].Add(false, [&]()
//		{
//			//IsSprint = false;
//			//LockOnCameraSettingMap[false]();
//			//if (CurEActionType == EActionType::MOVE &&
//			//	AnimInstance->PlayerAnimationType != AnimationType::ENDOFSPRINT &&
//			//	AnimInstance->PlayerAnimationType != AnimationType::HEAL &&
//			//	AnimInstance->PlayerAnimationType != AnimationType::SUPERHIT)
//			//{
//			//	PlayerCurAction = EPlayerState::RUN;
//			//	Run();
//			//}
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::ATTACK].Add(true, [&]()
//		{
//			//if (UseStamina(PlayerUseStaminaMap[EActionType::POWERATTACK]))
//			//{
//			//	PlayerCurAttackIndex++;
//			//	ChangActionType(EActionType::ATTACK);
//			//	ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
//			//	CanNextAttack = false;
//			//	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
//			//}
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::POWERATTACK].Add(true, [&]()
//		{
//			//if (UseStamina(PlayerUseStaminaMap[EActionType::POWERATTACK]))
//			//{
//			//	PlayerCurAttackIndex++;
//			//	ChangActionType(EActionType::ATTACK);
//			//	ChangeMontageAnimation(AnimationType::RUNPOWERATTACK);
//			//	CanNextAttack = false;
//			//	AObjectPool::GetInstance().SpawnObject(AObjectPool::GetInstance().ObjectArray[25].ObjClass, GetActorLocation(), FRotator::ZeroRotator);
//			//}
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::PARRING].Add(true, InputEventMap[EPlayerState::NONE][EActionType::PARRING][true]);
//	InputEventMap[EPlayerState::SPRINT][EActionType::MOVE].Add(true, [&]()
//		{
//			//ChangActionType(EActionType::MOVE);
//			////AnimInstance->BodyBlendAlpha = 0.0f;
//			//SetSpeed(SpeedMap[IsLockOn || IsGrab][true]);
//
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::MOVE].Add(false, [&]()
//		{
//			//if (AxisX == 1 && AxisY == 1)
//			//{
//			//	SetSpeed(0);
//			//	ChangeMontageAnimation(AnimationType::ENDOFSPRINT);
//			//	ChangActionType(EActionType::NONE);
//			//}
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::HEAL].Add(true, InputEventMap[EPlayerState::NONE][EActionType::HEAL][true]);
//	//InputEventMap[EPlayerState::SPRINT][EActionType::INTERACTION].Add(true, InputEventMap[EPlayerState::NONE][EActionType::INTERACTION][true]);
//	InputEventMap[EPlayerState::SPRINT][EActionType::SHIELD].Add(true, [&]()
//		{
//			//if (IsSprint && AnimInstance->PlayerAnimationType == AnimationType::SPRINT)
//			//{
//			//	InputEventMap[EPlayerState::SPRINT][EActionType::DODGE][false]();
//			//}
//			//InputEventMap[EPlayerState::NONE][EActionType::SHIELD][true]();
//			//
//			//if (IsGrab)
//			//	AnimInstance->BodyBlendAlpha = 0.0f;
//		});
//	InputEventMap[EPlayerState::SPRINT][EActionType::SHIELD].Add(false, InputEventMap[EPlayerState::NONE][EActionType::SHIELD][false]);

	for (EAnimationType AnimType : TEnumRange<EAnimationType>())
	{
		if (!MontageEndEventMap.Contains(AnimType))
		{
			MontageEndEventMap.Add(AnimType, [&]()
				{

				});
		}
	}

	for (EActionType MyActionType : TEnumRange<EActionType>())
	{
		if (!PlayerActionTickMap.Contains(MyActionType))
		{
			PlayerActionTickMap.Add(MyActionType, [&]()
				{

				});
		}
	}

	for (EPlayerState StateType : TEnumRange<EPlayerState>())
	{
		if (!InputEventMap.Contains(StateType))
		{
			InputEventMap.Add(StateType, TMap<EInputType, TMap<bool, TFunction<void()>>>());
		}

		for (EInputType InputType : TEnumRange<EInputType>())
		{
			if (!InputEventMap[StateType].Contains(InputType))
			{
				InputEventMap[StateType].Add(InputType, TMap<bool, TFunction<void()>>());
				InputEventMap[StateType][InputType].Add(true, [&]()
					{

					});
				InputEventMap[StateType][InputType].Add(false, [&]()
					{

					});
			}
		}
	}

	for (EAnimationType MyAnimType : TEnumRange<EAnimationType>())
	{
		for (bool Value = false; Value != true; Value = true)
		{
			if (NotifyEventMap[MyAnimType].Contains(Value))
			{
				NotifyEventMap[MyAnimType].Add(Value, [&]() {});
			}
		}
	}
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	SetSpeed(PlayerDataStruct.OriginSpeed);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{		
		EnhancedInputComponent->BindAction(InputActionMap[EInputType::MOVE], ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
		EnhancedInputComponent->BindAction(InputActionMap[EInputType::MOVE], ETriggerEvent::None, this, &AMyProjectCharacter::MoveEnd);

		EnhancedInputComponent->BindAction(InputActionMap[EInputType::ROTATE], ETriggerEvent::Triggered, this, &AMyProjectCharacter::Look);

		EnhancedInputComponent->BindAction(InputActionMap[EInputType::SPRINT], ETriggerEvent::Started, this, &AMyProjectCharacter::InputStarted);
		EnhancedInputComponent->BindAction(InputActionMap[EInputType::SPRINT], ETriggerEvent::None, this, &AMyProjectCharacter::InputEnded);

		EnhancedInputComponent->BindAction(InputActionMap[EInputType::LOCKON], ETriggerEvent::Started, this, &AMyProjectCharacter::LockOn);

		EnhancedInputComponent->BindAction(InputActionMap[EInputType::SPRINT], ETriggerEvent::Started, this, &AMyProjectCharacter::InputStarted);


		//BindAction<UInputBufferComponent, const UInputAction*, ETriggerEvent, const FVector>(Action, ETriggerEvent::Started, this, &UInputBufferComponent::OnBufferedAction, Action, ETriggerEvent::Started, moveInput)
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		YawRotation = FRotator(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProjectCharacter::MoveEnd()
{
	MovementVector = FVector2D(0.0f, 0.0f);
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookTarget();

	AnimInstance->SetSpeed(GetVelocity().Length());

	AnimInstance->SetAxisX(FMath::Lerp(AnimInstance->GetAxisX(), MovementVector.X, fDeltaTime * 10.0f));
	AnimInstance->SetAxisY(FMath::Lerp(AnimInstance->GetAxisY(), MovementVector.Y, fDeltaTime * 10.0f));
}

void AMyProjectCharacter::LookTarget()
{
	if (IsLockOn)
	{

	}
}

void AMyProjectCharacter::InputStarted()
{
	//InputEventMap[]
}

void AMyProjectCharacter::InputEnded()
{

}

void AMyProjectCharacter::LockOn()
{
	IsLockOn = !IsLockOn;

	GetCharacterMovement()->bOrientRotationToMovement = !IsLockOn;

	IsLockOn ? LockOnBegin() : LockOnEnd();
}

void AMyProjectCharacter::LockOnBegin()
{
	AnimInstance->SetLockOn(true);

	SetSpeed(PlayerDataStruct.PlayerLockOnMoveSpeed);
}

void AMyProjectCharacter::Attack()
{
	ComboAttackStart();
}

void AMyProjectCharacter::ComboAttackStart()
{
	CurStateType = EPlayerState::BEFOREATTACK;
	PlayerCurAttackIndex = 0;
	IsRotate = false;
}

void AMyProjectCharacter::LockOnEnd()
{
	AnimInstance->SetLockOn(false);

	if(!IsSprint)
	SetSpeed(PlayerDataStruct.OriginSpeed);
}

void AMyProjectCharacter::SprintBegin()
{
	SetSpeed(PlayerDataStruct.PlayerRunSpeed);
}

void AMyProjectCharacter::SprintEnd()
{
	SetSpeed(PlayerDataStruct.OriginSpeed);
}

void AMyProjectCharacter::PlayMontageAnimation(EAnimationType type)
{
	AnimInstance->Montage_Stop(0.2f, MontageMap[CurAnimType]);
	CurAnimType = type;
	AnimInstance->Montage_Play(MontageMap[CurAnimType]);
}

void AMyProjectCharacter::EventNotify(bool IsBegin)
{
	NotifyEventMap[CurAnimType][IsBegin]();
}