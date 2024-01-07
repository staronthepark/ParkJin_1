#include "MyObjectPool.h"

AMyObjectPool*  AMyObjectPool::Instance = nullptr;

AMyObjectPool::AMyObjectPool()
{
}

void AMyObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
	Init();

	Instance = this;
}

AMyObjectPool& AMyObjectPool::GetInstance()
{
	return *Instance;
}

void AMyObjectPool::Init()
{
	UWorld* World = GetWorld();

	for (int8 i = 0; i < ObjectArray.Num(); i++)
	{
		ObjectMap.Add(ObjectArray[i].Type, TArray<ABaseDynamicObject*>());
		for (int8 j = 0; j < ObjectArray[j].CreateCount; j++)
		{
			ABaseDynamicObject* NewObj = World->SpawnActor<ABaseDynamicObject>(ObjectArray[i].StaticClass);
			NewObj->SetObjectType(ObjectArray[i].Type);
			ObjectMap[ObjectArray[i].Type].Add(NewObj);
		}
	}

	ObjectArray.Empty();
}

void AMyObjectPool::SpawnObject(FVector Location, FRotator Rotation, EObjectType Type)
{
	ABaseDynamicObject* Object = nullptr;

	if (ObjectMap[Type].Num() <= 1) {
		UWorld* World = GetWorld();
		World->SpawnActor<ABaseDynamicObject>(ObjectMap[Type][0]->StaticClass());
		ObjectMap[Type].Add(World->SpawnActor<ABaseDynamicObject>(ObjectMap[Type][0]->StaticClass()));
	}

	Object = ObjectMap[Type].Pop();
	Object->Activate();
	Object->SetActorLocation(Location);
	Object->SetActorRotation(Rotation);	
}

void AMyObjectPool::ReturnObject(EObjectType Type, ABaseDynamicObject* Object)
{
	ObjectMap[Type].Push(Object);
}