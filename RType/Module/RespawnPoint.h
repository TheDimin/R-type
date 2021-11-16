#pragma once
#pragma once
#include "../Extension/Entity.h"
class RespawnPoint :
	public Entity
{
public:
	RespawnPoint();

	void Render(Tmpl8::Surface* Surface) const override;
	void Tick(float DeltaTime) override {};

protected:
	void OnEnable() override;
	//void OnDisable() override;
};