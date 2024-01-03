#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Character/BaseCharacter.h"
#include "CombatManager.generated.h"

UCLASS()
class MYPROJECT_API UCombatManager : public UObject
{
	GENERATED_BODY()
	
private:
	TArray<ABaseCharacter*> DamagedCharacters;

public:
	
	void Init();
	void ActivateHitCollision();

	void AddDamagedCharacter(ABaseCharacter* character) { DamagedCharacters.AddUnique(character); };
	void RemoveDamagedCharacter(ABaseCharacter* character) { DamagedCharacters.Remove(character); }
};