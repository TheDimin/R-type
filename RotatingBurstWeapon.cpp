#include "../precomp.h"
#include "RType/Weapons/RotatingBurstWeapon.h"
#include "Extension/Modules/Bullet/Projectile.h"


RotatingBurstWeapon::RotatingBurstWeapon(Entity& Owner) : WeaponDefinition(Owner)
{
	projectileSprite.Set(new Sprite(new Surface("assets/white.png"), 1));
	bulletSpeed = 150;
	counter = 0;
	maxCounter = 10;
	angle = 180;
	maxCoolDown = .2f;
}

void RotatingBurstWeapon::OnActivate(const Tmpl8::vec2& TargetPos)
{
	counter++;

	int finalCounter = static_cast<int>(static_cast<float>(maxCounter) * powf(sqrtf(static_cast<float>(level)), .9f));
	if (counter > finalCounter)
		counter = 0;

	float factor = static_cast<float>(counter) / static_cast<float>(finalCounter);

	SpawnProjectile(GetOrigin(),
		vec2(cos(factor * PI + PI * .5f) * angle, sin(factor * PI + PI * .5f) * angle));
}

void RotatingBurstWeapon::Init()
{
	currentCoolDown = 0;
}
