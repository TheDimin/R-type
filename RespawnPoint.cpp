#include "precomp.h"
#include "RType/Module/RespawnPoint.h"

#include "surface.h"
#include "Extension/Level.h"
#include "RType/Module/RespawnModule.h"

RespawnPoint::RespawnPoint() : Entity()
{
#ifndef SHOW_RESPAWN
	shouldRender = false;
#endif
}

void RespawnPoint::Render(Tmpl8::Surface* Surface) const
{
	vec2 tl = level->ToScreenSpace(pos - size * 0.5f);
	vec2 br = level->ToScreenSpace(pos + size * 0.5f);
	Surface->Box(tl, br, 0x00ffff);
}

void RespawnPoint::OnEnable()
{
	GameObject::OnEnable();
	level->GetModule<RespawnModule>()->SetRespawnPoint(pos);
	SetIsActive(false);
}
