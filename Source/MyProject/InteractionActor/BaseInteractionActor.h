#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Flag/Enums.h"
#include "Particles/ParticleSystemComponent.h"
#include "BaseInteractionActor.generated.h"

UCLASS()
class MYPROJECT_API ABaseInteractionActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EAnimationType PlayerAnimType;

	UPROPERTY(Editanywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UPrimitiveComponent* MyRootComponent;

	TArray<UFXSystemComponent*> InteractionCompArray;

public:	
	ABaseInteractionActor();

	virtual void BeginPlay() override;

	virtual void ExecuteEvent();

	FORCEINLINE virtual EAnimationType GetInteractionAnimType() { return PlayerAnimType; };
};