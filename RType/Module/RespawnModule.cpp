#include "../precomp.h"
#include "RespawnModule.h"
#include "../Extension/Level.h"

void RespawnModule::OnLoad()
{
	IModule::OnLoad();
}

void RespawnModule::SetRespawnPoint(const Tmpl8::vec2& newPoint)
{
	GameObject* p = (*lvl)->GetPlayer();
	if (!p->GetIsActive())
	{
		p->SetIsActive(true);
		p->pos = newPoint;
	}
	respawnPoint = newPoint;
}
