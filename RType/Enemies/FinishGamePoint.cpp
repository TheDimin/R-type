#include "../precomp.h"
#include "FinishGamePoint.h"

#include "../../Extension/Level.h"
#include "../RType/Levels/GameOver.h"

void FinishGamePoint::OnEnable()
{
	Entity::OnEnable();
	level->gameInstance->LoadLevel(new GameOver());
}
