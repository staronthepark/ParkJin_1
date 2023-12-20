// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDynamicObject.h"

// Sets default values
ABaseDynamicObject::ABaseDynamicObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseDynamicObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseDynamicObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

