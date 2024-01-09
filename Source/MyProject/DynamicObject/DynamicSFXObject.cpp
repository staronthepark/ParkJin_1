#include "../DynamicObject/DynamicSFXObject.h"

ADynamicSFXObject::ADynamicSFXObject()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>("Audio Comp");
	AudioComp->bAutoActivate = false;
}

void ADynamicSFXObject::BeginPlay()
{
	Super::BeginPlay();
}

void ADynamicSFXObject::Activate()
{
	Super::Activate();

	GetWorldTimerManager().SetTimer(Timer, this, &ADynamicSFXObject::ReturnObject, LifeTime);

	AudioComp->SetActive(true, true);
	AudioComp->Play();
}

void ADynamicSFXObject::Deactivate()
{
	Super::Deactivate();

	AudioComp->SetActive(false);
	AudioComp->Stop();
}

void ADynamicSFXObject::ReturnObject()
{
	Super::ReturnObject();
	GetWorldTimerManager().ClearTimer(Timer);
	AudioComp->Stop();
}