#pragma once
#include "../Extension/Modules/Bullet/WeaponDefinition.h"

class BurstWeapon : public WeaponDefinition
{
public:
	BurstWeapon(Entity& Owner);

protected:
	void OnActivate(const Tmpl8::vec2& TargetPos) override;

private:
	int amount;
	float spread;
};