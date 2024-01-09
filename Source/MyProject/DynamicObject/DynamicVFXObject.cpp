#include "../DynamicObject/DynamicVFXObject.h"


void ADynamicVFXObject::BeginPlay()
{
	Super::BeginPlay();

	ComponentArray = GetComponentsByClass(UPrimitiveComponent::StaticClass());
	Deactivate();
}

void ADynamicVFXObject::Activate()
{
	Super::Activate();

	GetWorldTimerManager().SetTimer(Timer, this, &ADynamicVFXObject::ReturnObject, LifeTime);

	for (int8 i = 0; i < ComponentArray.Num(); i++)
	{
		ComponentArray[i]->SetActive(true, true);
	}
}

void ADynamicVFXObject::Deactivate()
{
	Super::Deactivate();

	for (int8 i = 0; i < ComponentArray.Num(); i++)
	{
		ComponentArray[i]->SetActive(false, false);
	}
}


void ADynamicVFXObject::ReturnObject()
{
	Super::ReturnObject();
	GetWorldTimerManager().ClearTimer(Timer);
	for (int8 i = 0; i < ComponentArray.Num(); i++)
	{
		ComponentArray[i]->SetActive(false);
	}
}