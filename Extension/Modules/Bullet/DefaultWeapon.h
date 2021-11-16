#pragma once
#include "WeaponDefinition.h"

class DefaultWeapon :
	public WeaponDefinition
{
public:
	DefaultWeapon(Entity& Owner);
protected:
	void OnActivate(const vec2& TargetPos) override;
	void OnCollide(Projectile& Projectile, ICollider& HitCollider) override;
};

class DefaultPlayerWeapon :
	public DefaultWeapon
{
public:
	DefaultPlayerWeapon(Entity& Owner);
};