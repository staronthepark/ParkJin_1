#include "../Manager/CombatManager.h"

void UCombatManager::Init()
{
	DamagedCharacters.Empty();
}

void UCombatManager::ActivateHitCollision()
{
	for (int8 i = 0; i < DamagedCharacters.Num(); i++)
	{
		DamagedCharacters[i]->ActivateHitCollision(ECollisionEnabled::QueryOnly);
	}
}