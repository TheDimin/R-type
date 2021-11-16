#pragma once
#include "../Extension/Interfaces/IModule.h"
#include "../template.h"

namespace Tmpl8 {
	class vec2;
}


class RespawnModule : public IModule
{
public:
	void OnLoad() override;
	void SetRespawnPoint(const Tmpl8::vec2& newPoint);
	Tmpl8::vec2 GetRespawnPoint() const { return respawnPoint; }

private:
	Tmpl8::vec2 respawnPoint = { 0,0 };
};