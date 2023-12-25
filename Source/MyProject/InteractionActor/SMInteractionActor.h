#pragma once

#include "CoreMinimal.h"
#include "BaseInteractionActor.h"
#include "SMInteractionActor.generated.h"

UCLASS()
class MYPROJECT_API ASMInteractionActor : public ABaseInteractionActor
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* SMMesh;
public:

	ASMInteractionActor();

	virtual void BeginPlay() override;
	virtual void ExecuteEvent() override;
	
};