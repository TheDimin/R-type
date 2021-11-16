#include "../precomp.h"
#include "DefaultWeapon.h"
#include "Projectile.h"
#include "../Extension/Entity.h"

DefaultWeapon::DefaultWeapon(Entity& Owner) : WeaponDefinition(Owner)
{
	projectileSprite.Set(new Sprite(new Surface("assets/white.png"), 1));

	maxCoolDown = 2.f;
	minCoolDown = .8f;
	currentCoolDown = Rand(maxCoolDown) + minCoolDown;

	bulletSpeed = 200;
}

void DefaultWeapon::OnActivate(const vec2& TargetPos)
{
	WeaponDefinition::OnActivate(TargetPos);
	//currentCoolDown = (sqrtf(level) * -.4f) * maxCoolDown;
}

void DefaultWeapon::OnCollide(Projectile& Projectile, ICollider& HitCollider)
{
	if (Projectile.instigator != HitCollider.GetOwner())
		Projectile.SetIsActive(false);

	WeaponDefinition::OnCollide(Projectile, HitCollider);
}

DefaultPlayerWeapon::DefaultPlayerWeapon(Entity& Owner) : DefaultWeapon(Owner)
{
	maxCoolDown = .3f;
	bulletSpeed = 300;
	
}
