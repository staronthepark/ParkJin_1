#pragma once

#include "CoreMinimal.h"
#include "../DynamicObject/BaseDynamicObject.h"
#include "DynamicVFXObject.generated.h"

UCLASS()
class MYPROJECT_API ADynamicVFXObject : public ABaseDynamicObject
{
	GENERATED_BODY()

	TArray<UActorComponent*>ComponentArray;


public:

	virtual void BeginPlay() override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void ReturnObject() override;
};