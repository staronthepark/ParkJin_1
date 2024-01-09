// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Flag/Enums.h"
#include "BaseDynamicObject.generated.h"

UCLASS()
class MYPROJECT_API ABaseDynamicObject : public AActor
{
	GENERATED_BODY()
	
protected:
	FTimerHandle Timer;

	EObjectType Type;

	UPROPERTY(Editanywhere)
	float LifeTime;


public:	
	ABaseDynamicObject();

	virtual void BeginPlay();	
	virtual void Activate() {}
	virtual void Deactivate() {}
	virtual void ReturnObject();

	inline void SetObjectType(EObjectType _Type) { Type = _Type; }
};