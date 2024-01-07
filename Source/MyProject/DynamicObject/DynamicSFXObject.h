#pragma once

#include "CoreMinimal.h"
#include "../DynamicObject/BaseDynamicObject.h"
#include "Components/AudioComponent.h"
#include "DynamicSFXObject.generated.h"

UCLASS()
class MYPROJECT_API ADynamicSFXObject : public ABaseDynamicObject
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere)
	UAudioComponent* AudioComp;

public:
	ADynamicSFXObject();

	virtual void BeginPlay() override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void ReturnObject() override;
};