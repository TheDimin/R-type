#pragma once
#include "../Extension/Modules/Bullet/WeaponDefinition.h"

class RotatingBurstWeapon : public WeaponDefinition
{
public:
	RotatingBurstWeapon(Entity& Owner);

protected:
	virtual void OnActivate(const Tmpl8::vec2& TargetPos) override;
	void Init() override;

private:
	int counter = 0;
	int maxCounter = 25;
	float angle;
};