#include "BaseDynamicObject.h"
#include "../Manager/MyObjectPool.h"

ABaseDynamicObject::ABaseDynamicObject()
{
	PrimaryActorTick.bCanEverTick = false;	
}

void ABaseDynamicObject::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseDynamicObject::ReturnObject()
{
	Deactivate();
	AMyObjectPool::GetInstance().ReturnObject(Type, this);
}