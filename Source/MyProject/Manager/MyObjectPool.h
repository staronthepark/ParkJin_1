#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Flag/Enums.h"
#include "../DynamicObject/BaseDynamicObject.h"
#include "MyObjectPool.generated.h"


USTRUCT(BlueprintType)
struct FObjectInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Editanywhere)
	EObjectType Type;

	UPROPERTY(Editanywhere)
	TSubclassOf<ABaseDynamicObject> StaticClass;

	UPROPERTY(Editanywhere)
	int8 CreateCount;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API AMyObjectPool : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Editanywhere)
	TArray<FObjectInfo>ObjectArray;

	TMap<EObjectType, TArray<ABaseDynamicObject*>>ObjectMap;

	static AMyObjectPool* Instance;

public:	

	static AMyObjectPool& GetInstance();

	virtual void BeginPlay() override;

	void Init();
	void SpawnObject(FVector Location, FRotator Rotation, EObjectType Type);

	UFUNCTION()
	void ReturnObject(EObjectType Type, ABaseDynamicObject* Object);
};
