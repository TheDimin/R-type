#include "../precomp.h"
#include "../Extension/Modules/Bullet/Projectile.h"
#include "BurstWeapon.h"

BurstWeapon::BurstWeapon(Entity& Owner)
	: WeaponDefinition(Owner)
{
	projectileSprite.Set(new Sprite(new Surface("assets/white.png"), 1));
	bulletSpeed = 150;
	minCoolDown = 1.5f;
	maxCoolDown = 1.5f;
	currentCoolDown = Rand(maxCoolDown) + minCoolDown;

	amount = 3;
	spread = 300.f;
}

void BurstWeapon::OnActivate(const Tmpl8::vec2& TargetPos)
{
	const vec2 dir = CalculateDir(TargetPos);
	const vec2 origin = GetOrigin();
	for (int i = 0; i < amount; ++i)
	{
		const float offset = lerp(-spread, spread, static_cast<float>(i) / static_cast<float>(amount));

		//TODO offset has to be rotated by angle
		SpawnProjectile(origin, dir + vec2(0, offset + spread / static_cast<float>(amount)));
	}
}